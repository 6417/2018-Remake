/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj2.command.CommandScheduler;
import edu.wpi.first.wpilibj2.command.SequentialCommandGroup;
import frc.robot.commands.InitialCommand;
import frc.robot.commands.MoveCartToBack;
import frc.robot.commands.MoveCartToCenter;
import frc.robot.commands.MoveCartToFront;
import frc.robot.commands.Reset;
import frc.robot.commands.ZeroCart;
import frc.robot.subsystems.Cart;

public class Robot extends TimedRobot {
	private RobotContainer m_robotContainer;

	@Override
	public void robotInit() {
		m_robotContainer = new RobotContainer();
		Motors.init();
	}
	
	@Override
	public void robotPeriodic() {
		CommandScheduler.getInstance().run();
	}
	
	@Override
	public void disabledInit() {
	}
	
	@Override
	public void disabledPeriodic() {
	}
	
	@Override
	public void autonomousInit() {
	}
	
	@Override
	public void autonomousPeriodic() {
	}
	
	ZeroCart zeroCart = new ZeroCart();
	
	@Override
	public void teleopInit() {

	}
	
	@Override
	public void teleopPeriodic() {

	}
	
	@Override
	public void testInit() {
		CommandScheduler.getInstance().cancelAll();
	}

	@Override
	public void testPeriodic() {
	}
}
