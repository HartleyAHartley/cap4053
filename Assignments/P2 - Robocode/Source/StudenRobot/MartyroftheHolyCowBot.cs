using Robocode;
using Robocode.Util;
using System.Collections.Generic;
using System;
using System.Drawing;


namespace CAP4053.Student
{
	public class MartyroftheHolyCowBot : TeamRobot
	{

		private double direction = 1.0;
		private Dictionary<string, ScannedRobotEvent> opponents = new Dictionary<string, ScannedRobotEvent>();
		private Dictionary<string, double> prevEnergies = new Dictionary<string, double>();
		private string target = "";
		private double offset = Math.PI / 2;
		private double bulletPower = 3;
		private State currState = State.Searching;
		private long lastReverse = 0;
		private const long reverseCooldown = 1;

		private enum State
		{
			Searching,
			Found
		};

		private ScannedRobotEvent s
		{
			get
			{
				return opponents[target];
			}
		}

		private double energyDrop
		{
			get
			{
				switch (currState)
				{
					case State.Searching:
						return 0;
					case State.Found:
						return prevEnergies[target] - opponents[target].Energy;
					default:
						throw new Exception("Unexpected Enum");
				}
			}
		}

		private double radarAngle
		{
			get
			{
				switch (currState)
				{
					case State.Searching:
						return double.PositiveInfinity;
					case State.Found:
						return Utils.NormalRelativeAngle(HeadingRadians + s.BearingRadians - RadarHeadingRadians);
					default:
						throw new Exception("Unexpected Enum");
				}
			}
		}

		private double headOnAngle
		{
			get { return HeadingRadians + s.BearingRadians - GunHeadingRadians; }
		}
		private double deltaAngle
		{
			get { return Math.Sin(s.HeadingRadians - (HeadingRadians + s.BearingRadians)); }
		}
		private double predictedTraveltime
		{
			get { return s.Velocity / Rules.GetBulletSpeed(bulletPower); }
		}

		private double gunAngle
		{
			get
			{
				switch (currState)
				{
					case State.Searching: return 0;
					case State.Found: return Utils.NormalRelativeAngle(headOnAngle + deltaAngle * predictedTraveltime);
					default:
						throw new Exception("Unexpected Enum");
				}
			}
		}

		private double moveAngle
		{
			get
			{
				switch (currState)
				{
					case
						State.Searching:
						return 0;
					case
						State.Found:
						return Utils.NormalRelativeAngle(s.BearingRadians - HeadingRadians + offset);
					default:
						throw new Exception("Unexpected Enum");
				}
			}
		}


		private void movement()
		{
			if (energyDrop > 0)
			{
				SetTurnLeftRadians(moveAngle);
				SetAhead(double.PositiveInfinity * direction);
				direction *= -1;
			}
			else if (Energy > 20)
			{
				SetTurnLeftRadians(moveAngle);
				SetAhead(double.PositiveInfinity * direction);
			}
		}

		private void gun()
		{
			switch (currState)
			{
				case State.Searching:
					return;
				case State.Found:
					if (Energy > 0 && GunHeat == 0)
					{
						SetFire(bulletPower);
					}
					return;
				default:
					throw new Exception("Unexpected Enum");
			}
		}

		private void aim()
		{
			switch (currState)
			{
				case State.Searching:
					return;
				case State.Found:
					SetTurnRadarLeftRadians(-1.1 * radarAngle);
					SetTurnGunLeftRadians(-gunAngle);
					return;
				default:
					throw new Exception("Unexpected Enum");
			}
		}

		public override void OnRobotDeath(RobotDeathEvent e)
		{
			if (e.Name == target)
			{
				SetTurnRadarLeft(double.PositiveInfinity);
				target = "";
				currState = State.Searching;
			}
		}

		public override void OnScannedRobot(ScannedRobotEvent s)
		{
			if (IsTeammate(s.Name))
			{
				return;
			}

			if (currState == State.Searching)
			{
				currState = State.Found;
				opponents[s.Name] = s;
				target = s.Name;
			}

			prevEnergies[target] = opponents[s.Name].Energy;
			opponents[s.Name] = s;

			movement();
			gun();
			aim();

		}

		public override void Run()
		{
			IsAdjustRadarForGunTurn = true;
			IsAdjustRadarForRobotTurn = true;
			IsAdjustGunForRobotTurn = true;
			BodyColor = Color.AliceBlue;
			BulletColor = Color.FromArgb(128, 128, 128);
			RadarColor = Color.DarkOrchid;
			ScanColor = Color.DarkOrchid;

			SetTurnRadarLeftRadians(double.PositiveInfinity);

			for (; ; )
			{
				Scan();
				Execute();
			}
		}

	}
}
