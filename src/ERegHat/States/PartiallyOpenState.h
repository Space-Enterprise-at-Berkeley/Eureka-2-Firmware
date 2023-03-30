#pragma once

#include <Arduino.h>
#include <PIDController.h>
#include "ERegHat/StateMachine.h"
#include "ERegHat/HAL.h"
#include "ERegHat/Util.h"
#include "ERegHat/Packets.h"
#include "ERegHat/Config.h"

namespace StateMachine {

    class PartiallyOpenState {
        private:
        PIDController *innerController_ = Util::getInnerController();
        // Note: 1 rev on main shaft is 3200 counts
        // Encoder itself is 64CPM (including all edges)
        unsigned long lastPrint_;
        float angleSetpoint_;

        public:
        PartiallyOpenState();
        void init(float angleSetpoint);
        void setAngle(float angleSetpoint);
        void update();
    };
    
    PartiallyOpenState* getPartiallyOpenState();
}