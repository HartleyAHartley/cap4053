using Robocode;
using Robocode.Util;
using System;
using System.Drawing;
using System.Collections.Generic;

namespace CAP4053.Student
{
    
    public class StateMachine
    {
        private Dictionary<string, ScannedRobotEvent> opponents;
        private string target;
        private bool foundTarget = false;
        private double direction;

        private BackInTheBarnyardBot robot;
        private Action<Event> function;
        private MessageEvent lastFiredMessage;
        public StateMachine(BackInTheBarnyardBot bot)
        {
            robot = bot;
            lastFiredMessage = new MessageEvent("","FIRED");
            robot.BulletColor = Color.FromArgb(128, 128, 128);
            robot.RadarColor = Color.DarkOrchid;

            opponents = new Dictionary<string, ScannedRobotEvent>();
            robot.IsAdjustGunForRobotTurn = true;
            robot.IsAdjustRadarForGunTurn = true;
            robot.IsAdjustRadarForRobotTurn = true;


            opponents = new Dictionary<string, ScannedRobotEvent>();
            target = "";
            direction = 1;
            function = ScannedBot;
        }

        public void Update()
        {
            robot.TurnRadarRightRadians(Double.PositiveInfinity);

        }
        public void Dispatch(Event e)
        {
            if (e is ScannedRobotEvent) 
            { 
                Transition(ScannedBot);
            }
            else if (e is HitByBulletEvent)
            {
                Transition(HitByBullet);
            }
            else if (e is RobotDeathEvent)
            {
                Transition(RobotDeath);
            }
            else if (e is HitWallEvent)
            {
                Transition(HitWall);
            }
            else if (e is MessageEvent)
            {
                Transition(MessageRecieved);
            }
                
            function(e);
        }
        public void Transition(Action<Event> func)
        {
            function = func;
        }

        private void MessageRecieved(Event ev)
        {
            MessageEvent e = (MessageEvent)ev;

            if((string)e.Message == "FIRED" && robot.Time - lastFiredMessage.Time > 40)
            {
                direction *= 1;
                lastFiredMessage = e;
            }

        }
        private void ScannedBot(Event ev)
        {


            ScannedRobotEvent e = (ScannedRobotEvent)ev;
            if (!robot.IsTeammate(e.Name))
            {
                target = e.Name;
                if (!opponents.ContainsKey(target))
                {
                    opponents.Add(e.Name, e);
                }

                robot.SendMessage("BackInTheBarnyardBot", "FIRED");

                if (robot.Energy >= 20)
                {
                    Strafe(e);
                    LinearFiring(e);

                }
                else
                {
                    Ram(e);
                    //LinearFiring(e);
                }
            }
            else
            {
                Strafe(e);
            }
            
        }

        private void Ram(ScannedRobotEvent e)
        {
            robot.SetAhead(e.Distance);
            robot.SetTurnRightRadians(e.BearingRadians);
            robot.SetTurnGunRightRadians(Utils.NormalRelativeAngle(e.BearingRadians - (robot.GunHeadingRadians - robot.HeadingRadians)));
            robot.SetTurnRadarRightRadians(Utils.NormalRelativeAngle(e.BearingRadians - (robot.RadarHeadingRadians - robot.HeadingRadians)));
            robot.SetFire(Rules.MAX_BULLET_POWER);
            opponents[e.Name] = e;
            robot.Execute();
        }

        private void LinearFiring(ScannedRobotEvent e)
        {
            double absBearing = robot.HeadingRadians + e.BearingRadians;

            double predictedX = robot.X + (e.Distance * Math.Sin(absBearing));
            double predictedY = robot.Y + (e.Distance * Math.Cos(absBearing));

            double i = 1;
            while (i*11 < getDistance(robot.X, robot.Y, predictedX, predictedY))
            {
                predictedX += e.Velocity * Math.Sin(e.HeadingRadians);
                predictedY += e.Velocity * Math.Cos(e.HeadingRadians);
                
                i++;
            }

            robot.SetTurnGunRightRadians(Utils.NormalRelativeAngle(Utils.NormalAbsoluteAngle(Math.Atan2(predictedX - robot.X, predictedY - robot.Y)) - robot.GunHeadingRadians));
            robot.SetTurnRadarRightRadians(Utils.NormalRelativeAngle(absBearing - robot.RadarHeadingRadians));
            robot.SetFire(3);

            robot.Execute();
        }

        
        private void Strafe(ScannedRobotEvent e)
        {
            if (e.Name == target && e.Energy < opponents[target].Energy)
            {
                direction *= -1;
            }

            robot.SetAhead(e.Distance * direction);
            robot.SetTurnRightRadians(Utils.NormalRelativeAngle(e.BearingRadians + (Math.PI/2)));
            opponents[e.Name] = e;
        }
        private double getDistance(double x1, double y1, double x2, double y2)
        {
            return Math.Sqrt(Math.Pow((x1 - x2), 2) + Math.Pow((y1 - y2), 2));
        }

        private void HitByBullet(Event ev)
        {
            HitByBulletEvent e = (HitByBulletEvent)ev;
            if (!foundTarget)
            {
                robot.SetTurnGunRightRadians(Utils.NormalRelativeAngle(e.BearingRadians - (robot.GunHeadingRadians - robot.HeadingRadians)));
                robot.SetTurnRadarRightRadians(Utils.NormalRelativeAngle(e.BearingRadians - (robot.RadarHeadingRadians - robot.HeadingRadians)));
            }
        }

        private void RobotDeath(Event ev)
        {
            RobotDeathEvent e = (RobotDeathEvent)ev;
            if (e.Name == target)
            {
                target = "";
                foundTarget = false;
            }
        }

        private void HitWall(Event ev)
        {
            HitWallEvent e = (HitWallEvent)ev;


            direction *= -1;
            robot.SetAhead(100 * direction);
            robot.Execute();
        }

    }


    public class BackInTheBarnyardBot : TeamRobot
    {
        private StateMachine state;

        public override void OnHitByBullet(HitByBulletEvent evnt)
        {
            state.Dispatch(evnt);
        }

        public override void OnRobotDeath(RobotDeathEvent evnt)
        {
            state.Dispatch(evnt);
        }

        public override void OnMessageReceived(MessageEvent evnt)
        {
            state.Dispatch(evnt);
        }

        public override void OnHitWall(HitWallEvent evnt)
        {
            state.Dispatch(evnt);
        }

        public override void OnScannedRobot(ScannedRobotEvent evnt)
        {
            state.Dispatch(evnt);
        }

        public override void Run()
        {

            state = new StateMachine(this);

            while (true)
            {
                state.Update();
            }


            
        }

    }
}