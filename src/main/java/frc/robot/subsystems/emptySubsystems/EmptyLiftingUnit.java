/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot.subsystems.emptySubsystems;

import frc.robot.subsystems.LiftingUnit;

public class EmptyLiftingUnit extends LiftingUnit {
    @Override
    public void periodic() {
    }

    @Override
    public boolean getLimitSwitchBottom() {
        return false;
    }

    @Override
    public void moveto(int pos) {
    }

    @Override
    public void setVelocity(double speed) {
    }

    @Override
    public int getEncoderTicks() {
        return 0;
    }    
    
    @Override
    public int getEncoderVelocity() {
        return 0;
    }

    @Override
    public void stop() {
    }
}
