using Robocode;
using Robocode.Util;
using System.Collections.Generic;
using System.Drawing;
using System;

namespace CAP4053.Student
{
  struct RobotState
  {
    public Dictionary<String, double> lastOpponentEnergy;
    public Dictionary<String, (double, double)> knownTeammatePos;
    public int numOpponents;
    public int movingDir;
    public double fieldWidth;
    public double fieldHeight;
    public double lastFiredMessage;
  }

  class RobotUtils
  {
    public static double Hypot(double x, double y)
    {
      double distX = Math.Pow(x, 2);
      double distY = Math.Pow(y, 2);
      return Math.Sqrt(distX + distY);
    }

    public static (double, double) getScannedTarget(ScannedRobotEvent evnt, AdvancedRobot self)
    {
      return (evnt.Distance * Math.Cos(GetAbsoluteBearing(evnt, self)) + self.X, evnt.Distance * Math.Sin(GetAbsoluteBearing(evnt, self)) + self.Y);
    }

    public static double Lerp(double a, double b, double t)
    {
      return a + (Math.Min(t, 1) * (b - a));
    }

    public static bool InField(double width, double height, double x, double y)
    {
      bool verifyLeftBorderX = x > 30;
      bool verifyLeftBorderY = y > 30;
      bool verifyRightBorderX = x < width - 60;
      bool verifyRightBorderY = y < height - 60;

      return verifyLeftBorderX && verifyLeftBorderY && verifyRightBorderX && verifyRightBorderY;
    }

    public static (double, double) Project((double, double) coords, double angle, double mag)
    {
      var (x, y) = coords;
      return (x + Math.Sin(angle) * mag, y + Math.Cos(angle) * mag);
    }

    public static bool ProjectedInField(double x, double y, double width, double height, double angle, double mag)
    {
      var (dX, dY) = Project((x, y), angle, mag);

      return InField(width, height, dX, dY);
    }

    public static bool ProjectionSafeForTeamMates(Dictionary<String, (double ,double)> teammatesDict, double x, double y, double angle, double mag)
    {
      var (dX, dY) = Project((x, y), angle, mag);

      foreach (var teammate in teammatesDict)
      {
        var (tX, tY) = teammate.Value;
        if (Hypot(tX - dX, tY - dY) < 3.0)
        {
          return false;
        }
      }

      return true;
    }

    public static double GetAbsoluteBearing(ScannedRobotEvent evnt, AdvancedRobot r)
    {
      return r.HeadingRadians + evnt.BearingRadians;
    }
  }

  abstract class State
  {
    public abstract (RobotState, State) Exec(RobotState dataState, ScannedRobotEvent evnt, AdvancedRobot robot, TeamRobot team);
  }

  class IsTeammate : State
  {
    public override (RobotState, State) Exec(RobotState dataState, ScannedRobotEvent evnt, AdvancedRobot robot, TeamRobot team)
    {
      if (team.IsTeammate(evnt.Name))
      {
        return (dataState, new LogTeammate());
      } else
      {
        return (dataState, new ContainsKey());
      }
    }
  }

  class LogTeammate : State
  {
    public override (RobotState, State) Exec(RobotState dataState, ScannedRobotEvent evnt, AdvancedRobot robot, TeamRobot team)
    {
      if (!dataState.lastOpponentEnergy.ContainsKey(evnt.Name))
      {
        dataState.lastOpponentEnergy.Add(evnt.Name, evnt.Energy);
      }

      return (dataState, null);
    }
  }

  // Initalizes new Dictionary Entry if new Target
  class ContainsKey : State
  {
    public override (RobotState, State) Exec(RobotState dataState, ScannedRobotEvent evnt, AdvancedRobot robot, TeamRobot team)
    {
      if (!dataState.lastOpponentEnergy.ContainsKey(evnt.Name))
      {
        dataState.lastOpponentEnergy.Add(evnt.Name, evnt.Energy);
      }

      if (robot.Energy < 5)
      {
        return (dataState, new MoveCond());
      } else
      {
        return (dataState, new CheckGunHeat());
      }
    }
  }

  // Conditional Branch Dependent on Gun Heat
  class CheckGunHeat : State
  {
    public override (RobotState, State) Exec(RobotState dataState, ScannedRobotEvent _, AdvancedRobot robot, TeamRobot team)
    {
      if (robot.GunHeat < 1.2)
      {
        return (dataState, new RadarTrack());
      } else
      {
        return (dataState, new RadarSpin());
      }
    }
  }

  // Focuses radar on scan target
  class RadarTrack : State
  {
    public override (RobotState, State) Exec(RobotState dataState, ScannedRobotEvent evnt, AdvancedRobot robot, TeamRobot team)
    {
      robot.SetTurnRadarRightRadians(2 * Utils.NormalRelativeAngle(RobotUtils.GetAbsoluteBearing(evnt, robot) - robot.RadarHeadingRadians));
      return (dataState, new Fire());
    }
  }

  // Spins radar aimlessly
  class RadarSpin : State
  {
    public override (RobotState, State) Exec(RobotState dataState, ScannedRobotEvent _, AdvancedRobot robot, TeamRobot team)
    {
      robot.SetTurnRadarRightRadians(double.PositiveInfinity);
      return (dataState, new Fire());
    }
  }

  // Fires weapon
  class Fire : State
  {
    public override (RobotState, State) Exec(RobotState dataState, ScannedRobotEvent evnt, AdvancedRobot robot, TeamRobot team)
    {
      double firepower = 2 * RobotUtils.Lerp(0.7, 2, 300 / evnt.Distance);

      // Calculates angle offset using the lateral velocity of the opponent versus the velocity of the bullet
      double velocityProj = evnt.Velocity * Math.Sin(evnt.HeadingRadians - RobotUtils.GetAbsoluteBearing(evnt, robot)) / Rules.GetBulletSpeed(firepower);
      double gunHeadAngle = RobotUtils.GetAbsoluteBearing(evnt, robot) - robot.GunHeadingRadians;
      robot.SetTurnGunRightRadians(Utils.NormalRelativeAngle(gunHeadAngle + velocityProj));
      robot.SetFireBullet(firepower);

      return (dataState, new MoveCond());
    }
  }

  class MoveCond : State
  {
    public override (RobotState, State) Exec(RobotState dataState, ScannedRobotEvent evnt, AdvancedRobot robot, TeamRobot team)
    {
      if (dataState.lastOpponentEnergy[evnt.Name] > evnt.Energy)
      {
        dataState.movingDir = -dataState.movingDir;
        return (dataState, new Orbit());
      }

      if (robot.Energy < 5)
      {
        return (dataState, new TrackBeforeRam());
      } else
      {
        return (dataState, new Orbit());
      }
    }
  }

  class Orbit : State
  {
    public override (RobotState, State) Exec(RobotState dataState, ScannedRobotEvent evnt, AdvancedRobot robot, TeamRobot team)
    {
      // Spiral Movement (Perpendicular plus spiral-in and out options)
      double circleOpponent = robot.HeadingRadians + evnt.BearingRadians + (Math.PI / 2) * dataState.movingDir + (Math.PI / 6);

      // Avoid walls
      while (!RobotUtils.ProjectedInField(robot.X, robot.Y, dataState.fieldWidth, dataState.fieldHeight, circleOpponent, 120) && !RobotUtils.ProjectionSafeForTeamMates(dataState.knownTeammatePos, robot.X, robot.Y, circleOpponent, 120))
      {
        circleOpponent += dataState.movingDir * 0.1;
      }

      double angle = Utils.NormalRelativeAngle(circleOpponent - robot.HeadingRadians);
      if (Math.Abs(angle) > Math.PI / 2)
      {
        angle = Utils.NormalRelativeAngle(angle + Math.PI);
        robot.SetBack(100);
      }
      else
      {
        robot.SetAhead(100);
      }

      robot.SetTurnRightRadians(angle);

      return (dataState, new RecordOpponentEnergy());
    }
  }

  class TrackBeforeRam : State
  {
    public override (RobotState, State) Exec(RobotState dataState, ScannedRobotEvent evnt, AdvancedRobot robot, TeamRobot team)
    {
      robot.SetTurnRadarRightRadians(2 * Utils.NormalRelativeAngle(RobotUtils.GetAbsoluteBearing(evnt, robot) - robot.RadarHeadingRadians));
      return (dataState, new Ram());
    }
  }

  class Ram : State
  {
    public override (RobotState, State) Exec(RobotState dataState, ScannedRobotEvent evnt, AdvancedRobot robot, TeamRobot team)
    {
      robot.SetTurnRightRadians(Utils.NormalRelativeAngle(evnt.BearingRadians));
      robot.SetAhead(150);

      return (dataState, new RecordOpponentEnergy());
    }
  }

  class RecordOpponentEnergy : State
  {
    public override (RobotState, State) Exec(RobotState dataState, ScannedRobotEvent evnt, AdvancedRobot robot, TeamRobot team)
    {
      team.BroadcastMessage("FIRED");
      dataState.lastOpponentEnergy[evnt.Name] = evnt.Energy;

      return (dataState, null);
    }
  }

  public class DancingPolishCowBot : TeamRobot 
  {

    RobotState dataState = new RobotState
    {
      movingDir = 1,
      lastOpponentEnergy = new Dictionary<string, double>(),
      knownTeammatePos = new Dictionary<string, (double, double)>()
    };

    public override void OnStatus(StatusEvent e)
    {
      dataState.numOpponents = e.Status.Others;
    }

    public override void OnRobotDeath(RobotDeathEvent evnt)
    {
      dataState.lastOpponentEnergy.Remove(evnt.Name);
      dataState.knownTeammatePos.Remove(evnt.Name);
    }

    public override void OnMessageReceived(MessageEvent evnt)
    {
      if ((string)evnt.Message == "FIRED" && Time - dataState.lastFiredMessage > 15)
      {
        dataState.movingDir *= -1;
        dataState.lastFiredMessage = Time;
      }
    }


    public override void Run() 
    {
      IsAdjustGunForRobotTurn = true;
      IsAdjustRadarForGunTurn = true;

      BodyColor = Color.Aquamarine;
      BulletColor = Color.FromArgb(128, 128, 128);
      RadarColor = Color.DarkOrchid;
      ScanColor = Color.FromArgb(59, 99, 145);

      dataState.fieldWidth = BattleFieldWidth;
      dataState.fieldHeight = BattleFieldHeight;

      while (true) 
      {
        TurnRadarRightRadians(100);
      }
    }

    void runStateMachine(ScannedRobotEvent evnt, State startingState)
    {
      State nextState = startingState;
      while (nextState != null)
      {
        (dataState, nextState) = nextState.Exec(dataState, evnt, this, this);
      }
    }

    public override void OnScannedRobot(ScannedRobotEvent evnt)
    {
      runStateMachine(evnt, new ContainsKey());
    }
  }
}
