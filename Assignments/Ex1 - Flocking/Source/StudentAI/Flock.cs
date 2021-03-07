using System.Collections.Generic;
using AI.SteeringBehaviors.Core;

namespace AI.SteeringBehaviors.StudentAI
{
  public class Flock
  {
    public float AlignmentStrength { get; set; }
    public float CohesionStrength { get; set; }
    public float SeparationStrength { get; set; }
    public List<MovingObject> Boids { get; protected set; }
    public Vector3 AveragePosition { get; set; }
    protected Vector3 AverageForward { get; set; }
    public float FlockRadius { get; set; }

    public Flock() 
    {
      AverageForward = new Vector3(0.0f, 0.0f, 0.0f);
      AveragePosition = new Vector3(0.0f, 0.0f, 0.0f);
    }

    virtual public void Update(float deltaTime)
    {
      AverageForward = GetAvgForward();
      AveragePosition = GetAvgPosition();

      foreach(MovingObject boid in Boids)
      {
        Vector3 accel = calcAlignmentAccel(boid);
        accel = accel + calcCohesionAccel(boid);
        accel = accel + calcSeperationAccel(boid);
        accel = accel * boid.MaxSpeed * deltaTime;
        boid.Velocity = boid.Velocity + accel;

        if (boid.Velocity.Length > boid.MaxSpeed) {
          boid.Velocity.Normalize();
          boid.Velocity = boid.Velocity * boid.MaxSpeed;
        }

        boid.Update(deltaTime);

      }
    }

    private Vector3 GetAvgForward()
    {
      Vector3 avg = new Vector3(0.0f, 0.0f, 0.0f);
      foreach(MovingObject boid in Boids)
      {
        avg = avg + boid.Velocity;
      }
      return avg / Boids.Count;
    }

    private Vector3 GetAvgPosition()
    {
      Vector3 avg = new Vector3(0.0f, 0.0f, 0.0f);
      foreach (MovingObject boid in Boids)
      {
        avg = avg + boid.Position;
      }
      return avg / Boids.Count;
    }

    private Vector3 calcAlignmentAccel(MovingObject boid)
    {
      Vector3 accel = AverageForward / boid.MaxSpeed;
      if(accel.Length > 1)
      {
        accel.Normalize();
      }
      return accel * AlignmentStrength;
    }

    private Vector3 calcCohesionAccel(MovingObject boid)
    {
      Vector3 accel = AveragePosition - boid.Position;
      float distance = accel.Length;
      accel.Normalize();

      if (distance < FlockRadius){
        accel = accel * distance / FlockRadius;
      }
      return accel * CohesionStrength;
    }

    private Vector3 calcSeperationAccel(MovingObject boid)
    {
      Vector3 sum = new Vector3(0.0f, 0.0f, 0.0f);
      foreach (MovingObject sibling in Boids)
      {
        if(sibling == boid)
        {
          continue;
        }

        Vector3 accel = boid.Position - sibling.Position;
        float distance = accel.Length;
        float safeDistance = boid.SafeRadius + sibling.SafeRadius;

        if (distance < safeDistance){
          accel.Normalize();
          accel = accel * (safeDistance - distance) / safeDistance;
          sum = sum + accel;
        }
      }

      if (sum.Length > 1.0)
      {
        sum.Normalize();
      }

      return sum * SeparationStrength;
    }
  }
}
