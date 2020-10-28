package frc.robot.subsystems.emptySubsystems;

import frc.robot.subsystems.Cart;

public class EmptyCart extends Cart {
    
    @Override
    public void periodic() {     
    }

    @Override
    public void moveto(int pos){
    }

    @Override
    public boolean getLimitSwitchFront() {
        return false;
    }

    @Override
    public boolean getLimitSwitchBack() {
        return false;
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

    @Override
    public void setVelocity(double v) {
    }
}
