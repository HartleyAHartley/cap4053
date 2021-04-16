﻿using Robocode;
using Robocode.Util;
using System.Collections.Generic;
using System;
using System.Drawing;


namespace CAP4053.Student{
  
public class MartyroftheHolyCowBot : TeamRobot{
  
  private double direction = 1.0;
  private Dictionary<string, ScannedRobotEvent> opponents = new Dictionary<string, ScannedRobotEvent>();
  private Dictionary<string, double> prevEnergies = new Dictionary<string, double>();
  private string target = "";
  private double offset = Math.PI/2;
  private double bulletPower = 3;
  private State currState = State.Searching;
  private long lastReverse = 0;
  private const long reverseCooldown = 1;

  private enum State {
    Searching,
    Found
  };

  private ScannedRobotEvent s {
    get {
      return opponents[target];
    }
  }

  private double energyDrop{
    get {
      return opponents[target].Energy - prevEnergies[target];
    }
  }

  private double radarAngle{
    get {
      switch(currState){
        case State.Searching: 
          return double.PositiveInfinity;
        case State.Found: 
          return Utils.NormalRelativeAngle(HeadingRadians + s.BearingRadians - RadarHeadingRadians);
        default:
          throw new Exception("Unexpected Enum");
      }
    }
  }

  private double headOnAngle{
    get { return HeadingRadians + s.BearingRadians -	GunHeadingRadians; }
  }
  private double deltaAngle{
    get { return Math.Sin(s.HeadingRadians - (HeadingRadians + s.BearingRadians)); }
  }
  private double predictedTraveltime{
    get { return s.Velocity/Rules.GetBulletSpeed(3); }
  }

  private double gunAngle{
    get {
      switch(currState){
        case State.Searching: return 0;
        case State.Found: return Utils.NormalRelativeAngle(headOnAngle + deltaAngle * predictedTraveltime);
        default:
          throw new Exception("Unexpected Enum");
      }
    }
  }

  private double moveAngle{
    get{
      switch(currState){
        case 
          State.Searching: return 0;
        case 
          State.Found: return Utils.NormalRelativeAngle(s.BearingRadians - HeadingRadians + offset);
        default:
          throw new Exception("Unexpected Enum");
      }
    }
  }

  public override void OnRobotDeath(RobotDeathEvent e){
    if(e.Name == target){
      currState = State.Searching;
      SetTurnRadarLeft(double.PositiveInfinity);
      target = "";
    }
  }

  private void movement(){
    if (energyDrop > 0 && (Time - lastReverse) > reverseCooldown){
      SetTurnLeftRadians(moveAngle);
      SetAhead(32 * direction);
      direction *= -1;
      lastReverse = Time;
    }else if (Energy > 20){
      SetTurnLeftRadians(moveAngle);
      SetAhead(double.PositiveInfinity * direction);
    }
  }


  private void doNothing(double _){}
  private Action<double> bang {
    get {
      if(Energy > 0 && GunHeat == 0){
        return SetFire;
      }
      return doNothing;
    }
  }

  private void gun(){
    switch(currState){
      case State.Searching: 
        return;
      case State.Found: 
        bang(bulletPower);
        return;
      default:
        throw new Exception("Unexpected Enum");
    }
  }

  private void aim(){
    switch(currState){
      case State.Searching: 
        return;
      case State.Found:       
        SetTurnRadarLeftRadians(-1.1* radarAngle);
        SetTurnGunLeftRadians(-gunAngle);
        return;
      default:
        throw new Exception("Unexpected Enum");
    }
  }

  public override void OnScannedRobot(ScannedRobotEvent s){
    if (IsTeammate(s.Name)){
      return;
    }

    if(currState == State.Searching){
      currState = State.Found;
      target = s.Name;
    }

    prevEnergies[s.Name] = opponents.ContainsKey(s.Name) ? opponents[s.Name].Energy : s.Energy;
    opponents[s.Name] = s;
  }

  public override void OnMessageReceived(MessageEvent e){
    if(!(e.Message is string)){
      return;
    }
    if((string)e.Message != "FIRED"){
      return;
    }
    if((Time - lastReverse) > reverseCooldown){
      direction *= -1;
      lastReverse = Time;
    }
  }

  public override void Run(){
    init();

    for (;; ){
      Scan();
      aim();
      gun();
      Execute();
    }
  }

  private void init(){
    IsAdjustRadarForGunTurn = true;
    IsAdjustRadarForRobotTurn = true;
    IsAdjustGunForRobotTurn = true;
    BodyColor = Color.AliceBlue;
    BulletColor = Color.FromArgb(128, 128, 128);
    RadarColor = Color.DarkOrchid;
    ScanColor = Color.DarkOrchid;
  }
}
}
