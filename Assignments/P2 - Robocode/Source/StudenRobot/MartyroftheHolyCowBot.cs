using Robocode;
using Robocode.Util;
using System.Collections.Generic;
using System;
using System.Drawing;


namespace CAP4053.Student
{
	public class MartyroftheHolyCowBot : TeamRobot
	{
		public override void OnRobotDeath(RobotDeathEvent e)
		{
			if(e.Name == target)
      {
				SetTurnRadarLeft(double.PositiveInfinity);
				target = "";
      }
		}

		public override void OnScannedRobot(ScannedRobotEvent e)
		{
			var s = new Scanned(e,Time);
      if (IsTeammate(e.Name))
      {
				return;
      }
			if (target == "")
			{
				target = s.Name;
      }
      else if(target != s.Name)
      {
				opponents[s.Name] = s;
				return;
			}
      if (!opponents.ContainsKey(s.Name))
      {
				opponents[s.Name] = s;
			}

			var radarAngle = Utils.NormalRelativeAngle(HeadingRadians + s.BearingRadians - RadarHeadingRadians);
			var gunAngle = Utils.NormalRelativeAngle(HeadingRadians + s.BearingRadians - GunHeadingRadians + (s.Velocity*Math.Sin(s.HeadingRadians-(HeadingRadians + s.BearingRadians))/ Rules.GetBulletSpeed(3) ));
			var offset = Math.PI / 2;
			var moveAngle = Utils.NormalRelativeAngle(s.BearingRadians - HeadingRadians + offset);

			if (opponents[s.Name].Energy - e.Energy > 0)
			{
				SetTurnLeftRadians(moveAngle);
				SetAhead(double.PositiveInfinity * direction);
				direction *= -1;
			}else if (Energy > 20)
			{
				SetTurnLeftRadians(moveAngle);
				SetAhead(double.PositiveInfinity * direction);
			}
			if (Energy > 0 && GunHeat == 0)
      {
				SetFire(Rules.MAX_BULLET_POWER);
      }

			SetTurnRadarLeftRadians(-1.1* radarAngle);
			SetTurnGunLeftRadians(-gunAngle);


			opponents[s.Name] = s;
		}

		public override void Run()
		{
			opponents = new Dictionary<string, Scanned>();
			direction = 1.0;
			IsAdjustRadarForGunTurn = true;
			IsAdjustRadarForRobotTurn = true;
			IsAdjustGunForRobotTurn = true;
			BodyColor = Color.AliceBlue;
			BulletColor = Color.FromArgb(128, 128, 128);
			RadarColor = Color.DarkOrchid;
			ScanColor = Color.DarkOrchid;

			target = "";
			SetTurnRadarLeftRadians(double.PositiveInfinity);

			for (; ; )
			{
				Scan();
				Execute();
			}
		}

		private double direction;
		private Dictionary<String, Scanned> opponents;
		private String target;

    public class Scanned
    {
      public Scanned(ScannedRobotEvent e, long t)
      {
				Bearing = e.Bearing;
				BearingRadians = e.BearingRadians;
				Distance = e.Distance;
				Energy = e.Energy;
				Heading = e.Heading;
				HeadingRadians = e.HeadingRadians;
				Name = e.Name;
				Velocity = e.Velocity;
				IsSentryRobot = e.IsSentryRobot;
				time = t;
      }
			public double Bearing;
			public double BearingRadians;
			public double Distance;
			public double Energy;
			public double Heading;
			public double HeadingRadians;
			public string Name;
			public double Velocity;
			public bool IsSentryRobot;
			public long time;
		}
  }
}
