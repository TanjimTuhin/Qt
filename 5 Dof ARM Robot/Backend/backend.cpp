// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "backend.h"
#include <QTransform>
#include <QPolygon>
#include <QRect>
#include <QtMath>
#include <QTimer>

// Joint limits for the new 32-component robotic arm (in degrees)
const Backend::JointLimits Backend::s_jointLimits[6] = {
    {-180, 180},  // Joint 1: Base rotation
    {-90, 90},    // Joint 2: Shoulder pitch
    {-135, 135},  // Joint 3: Elbow pitch
    {-180, 180},  // Joint 4: Wrist roll
    {-90, 90},    // Joint 5: Wrist pitch
    {-180, 180}   // Joint 6: Wrist yaw
};

Backend::Backend(QObject *parent) : QObject(parent)
{
    // Connect the valueChanged signal from each AnimatedParam to the corresponding NOTIFY signal
    connect(&m_joint1Rotation, &AnimatedParam::valueChanged, this, &Backend::joint1RotationChanged);
    connect(&m_joint2Rotation, &AnimatedParam::valueChanged, this, &Backend::joint2RotationChanged);
    connect(&m_joint3Rotation, &AnimatedParam::valueChanged, this, &Backend::joint3RotationChanged);
    connect(&m_joint4Rotation, &AnimatedParam::valueChanged, this, &Backend::joint4RotationChanged);
    connect(&m_joint5Rotation, &AnimatedParam::valueChanged, this, &Backend::joint5RotationChanged);
    connect(&m_joint6Rotation, &AnimatedParam::valueChanged, this, &Backend::joint6RotationChanged);
    connect(&m_gripperAngle, &AnimatedParam::valueChanged, this, &Backend::gripperAngleChanged);

    // Connect to motion state updates
    connect(&m_joint1Rotation, &AnimatedParam::valueChanged, this, &Backend::updateMovingState);
    connect(&m_joint2Rotation, &AnimatedParam::valueChanged, this, &Backend::updateMovingState);
    connect(&m_joint3Rotation, &AnimatedParam::valueChanged, this, &Backend::updateMovingState);
    connect(&m_joint4Rotation, &AnimatedParam::valueChanged, this, &Backend::updateMovingState);
    connect(&m_joint5Rotation, &AnimatedParam::valueChanged, this, &Backend::updateMovingState);
    connect(&m_joint6Rotation, &AnimatedParam::valueChanged, this, &Backend::updateMovingState);
    connect(&m_gripperAngle, &AnimatedParam::valueChanged, this, &Backend::updateMovingState);

    // Bind the status property to update automatically based on arm state
    m_status.setBinding([this]() {
        if (m_isCollision.value())
            return QString("⚠️ Collision Detected!");

        if (m_isMoving.value()) {
            return QString("🔄 Moving...");
        }

        return QString("✅ Ready");
    });

    // Bind the moving state
    m_isMoving.setBinding([this]() {
        return m_joint1Rotation.isRunning() || m_joint2Rotation.isRunning() ||
               m_joint3Rotation.isRunning() || m_joint4Rotation.isRunning() ||
               m_joint5Rotation.isRunning() || m_joint6Rotation.isRunning() ||
               m_gripperAngle.isRunning();
    });

    // Connect all relevant joint changes to the collision detection and kinematics
    connect(&m_joint1Rotation, &AnimatedParam::valueChanged, this, &Backend::detectCollision);
    connect(&m_joint2Rotation, &AnimatedParam::valueChanged, this, &Backend::detectCollision);
    connect(&m_joint3Rotation, &AnimatedParam::valueChanged, this, &Backend::detectCollision);
    connect(&m_joint4Rotation, &AnimatedParam::valueChanged, this, &Backend::detectCollision);
    connect(&m_joint5Rotation, &AnimatedParam::valueChanged, this, &Backend::detectCollision);
    connect(&m_joint6Rotation, &AnimatedParam::valueChanged, this, &Backend::detectCollision);

    connect(&m_joint1Rotation, &AnimatedParam::valueChanged, this, &Backend::updateEndEffectorPosition);
    connect(&m_joint2Rotation, &AnimatedParam::valueChanged, this, &Backend::updateEndEffectorPosition);
    connect(&m_joint3Rotation, &AnimatedParam::valueChanged, this, &Backend::updateEndEffectorPosition);
    connect(&m_joint4Rotation, &AnimatedParam::valueChanged, this, &Backend::updateEndEffectorPosition);
    connect(&m_joint5Rotation, &AnimatedParam::valueChanged, this, &Backend::updateEndEffectorPosition);
    connect(&m_joint6Rotation, &AnimatedParam::valueChanged, this, &Backend::updateEndEffectorPosition);

    // Initialize end effector position
    updateEndEffectorPosition();
}

// --- Getters ---
int Backend::joint1Rotation() const { return m_joint1Rotation.value(); }
int Backend::joint2Rotation() const { return m_joint2Rotation.value(); }
int Backend::joint3Rotation() const { return m_joint3Rotation.value(); }
int Backend::joint4Rotation() const { return m_joint4Rotation.value(); }
int Backend::joint5Rotation() const { return m_joint5Rotation.value(); }
int Backend::joint6Rotation() const { return m_joint6Rotation.value(); }
int Backend::gripperAngle() const { return m_gripperAngle.value(); }
QString Backend::status() const { return m_status.value(); }
QBindable<QString> Backend::bindableStatus() const { return &m_status; }
bool Backend::isMoving() const { return m_isMoving.value(); }
bool Backend::hasCollision() const { return m_isCollision.value(); }
QVector3D Backend::endEffectorPosition() const { return m_endEffectorPosition.value(); }

// --- Setters ---
void Backend::setJoint1Rotation(int angle) {
    if (isWithinJointLimits(0, angle)) {
        m_joint1Rotation.setValue(angle);
    }
}

void Backend::setJoint2Rotation(int angle) {
    if (isWithinJointLimits(1, angle)) {
        m_joint2Rotation.setValue(angle);
    }
}

void Backend::setJoint3Rotation(int angle) {
    if (isWithinJointLimits(2, angle)) {
        m_joint3Rotation.setValue(angle);
    }
}

void Backend::setJoint4Rotation(int angle) {
    if (isWithinJointLimits(3, angle)) {
        m_joint4Rotation.setValue(angle);
    }
}

void Backend::setJoint5Rotation(int angle) {
    if (isWithinJointLimits(4, angle)) {
        m_joint5Rotation.setValue(angle);
    }
}

void Backend::setJoint6Rotation(int angle) {
    if (isWithinJointLimits(5, angle)) {
        m_joint6Rotation.setValue(angle);
    }
}

void Backend::setGripperAngle(int angle) {
    // Gripper limits: 0-45 degrees
    angle = qBound(0, angle, 45);
    m_gripperAngle.setValue(angle);
}

// --- Convenience Methods ---
void Backend::moveToHome() {
    setAllJoints(0, 0, 0, 0, 0, 0);
    setGripperAngle(0);
}

void Backend::moveToPickPosition() {
    setAllJoints(0, -45, 90, 0, -45, 0);
    setGripperAngle(30);
}

void Backend::moveToRestPosition() {
    setAllJoints(0, 75, -110, 0, 35, 0);
    setGripperAngle(0);
}

void Backend::moveToServicePosition() {
    setAllJoints(90, 0, -90, 90, 0, 0);
    setGripperAngle(15);
}

void Backend::openGripper() {
    setGripperAngle(45);
}

void Backend::closeGripper() {
    setGripperAngle(0);
}

void Backend::stopAllMotion() {
    // Stop all animated parameters
    // Note: This would depend on your AnimatedParam implementation
    // You might need to add a stop() method to AnimatedParam
    emit emergencyStop();
}

void Backend::setAllJoints(int j1, int j2, int j3, int j4, int j5, int j6) {
    if (isPositionSafe(j1, j2, j3, j4, j5, j6)) {
        setJoint1Rotation(j1);
        setJoint2Rotation(j2);
        setJoint3Rotation(j3);
        setJoint4Rotation(j4);
        setJoint5Rotation(j5);
        setJoint6Rotation(j6);
    }
}

void Backend::setJointAnglesSmooth(int j1, int j2, int j3, int j4, int j5, int j6, int duration) {
    // This would set animation duration for AnimatedParam
    // Implementation depends on your AnimatedParam class
    setAllJoints(j1, j2, j3, j4, j5, j6);
}

// --- Safety and Validation ---
bool Backend::isPositionSafe(int j1, int j2, int j3, int j4, int j5, int j6) const {
    // Check joint limits
    if (!isWithinJointLimits(0, j1) || !isWithinJointLimits(1, j2) ||
        !isWithinJointLimits(2, j3) || !isWithinJointLimits(3, j4) ||
        !isWithinJointLimits(4, j5) || !isWithinJointLimits(5, j6)) {
        return false;
    }

    // Additional safety checks can be added here
    // For example, checking for known collision-prone configurations

    return true;
}

bool Backend::isWithinJointLimits(int joint, int angle) const {
    if (joint < 0 || joint >= 6) return false;

    return angle >= s_jointLimits[joint].min && angle <= s_jointLimits[joint].max;
}

// --- Private Methods ---
void Backend::detectCollision() {
    bool collision = false;

    // Enhanced collision detection for the new 32-component arm
    collision |= checkSelfCollision();
    collision |= checkWorkspaceLimits();
    collision |= checkBaseCollisions();
    collision |= checkArmSegmentCollisions();
    collision |= checkWristCollisions();
    collision |= checkGripperCollisions();

    if (collision != m_isCollision.value()) {
        m_isCollision.setValue(collision);
        emit hasCollisionChanged();

        if (collision) {
            emit emergencyStop();
        }
    }
}

bool Backend::checkSelfCollision() const {
    // Simplified self-collision detection
    // This is a basic implementation - you may need more sophisticated algorithms

    // Check for extreme joint combinations that could cause self-collision
    if (abs(joint2Rotation()) > 60 && abs(joint3Rotation()) > 100) {
        return true; // Shoulder and elbow combination might cause collision
    }

    if (joint1Rotation() != 0 && abs(joint2Rotation()) < -70) {
        return true; // Base rotation with low shoulder might hit base
    }

    return false;
}

bool Backend::checkWorkspaceLimits() const {
    QVector3D pos = endEffectorPosition();
    double distance = pos.length();

    // Check if end effector is within workspace
    return distance > workspaceRadius();
}

bool Backend::checkBaseCollisions() const {
    // Check if any arm segment collides with the base
    // This would require more detailed geometric calculations
    return false; // Placeholder
}

bool Backend::checkArmSegmentCollisions() const {
    // Check collisions between arm segments (Art1, Art2, Art3, etc.)
    // This would require detailed 3D collision detection
    return false; // Placeholder
}

bool Backend::checkWristCollisions() const {
    // Check wrist joint collisions (Art4, Art5, Art6)
    return false; // Placeholder
}

bool Backend::checkGripperCollisions() const {
    // Check gripper collisions with other parts
    return false; // Placeholder
}

void Backend::updateEndEffectorPosition() {
    calculateForwardKinematics();
}

void Backend::calculateForwardKinematics() {
    // Forward kinematics calculation for the new 32-component arm
    // This follows the DH parameters from your merged robotic arm

    // Link lengths and offsets (adjust based on your actual robot dimensions)
    const double L1 = 0.084;  // Base to shoulder height
    const double L2 = 0.173;  // Shoulder to elbow length
    const double L3 = 0.089;  // Elbow forward offset
    const double L4 = 0.169;  // Elbow to wrist height
    const double L5 = 0.038;  // Wrist segment 1
    const double L6 = 0.038;  // Wrist segment 2
    const double L7 = 0.036;  // Wrist to gripper

    // Convert angles to radians
    double q1 = qDegreesToRadians(joint1Rotation());
    double q2 = qDegreesToRadians(joint2Rotation());
    double q3 = qDegreesToRadians(joint3Rotation());
    double q4 = qDegreesToRadians(joint4Rotation());
    double q5 = qDegreesToRadians(joint5Rotation());
    double q6 = qDegreesToRadians(joint6Rotation());

    // Forward kinematics calculation
    // This is a simplified version - you might need a more detailed implementation

    double x = cos(q1) * (L2 + L3*cos(q2) + L4*cos(q2+q3));
    double y = L1 + L3*sin(q2) + L4*sin(q2+q3) + L5 + L6 + L7;
    double z = sin(q1) * (L2 + L3*cos(q2) + L4*cos(q2+q3));

    QVector3D newPosition(x, y, z);

    if (newPosition != m_endEffectorPosition.value()) {
        m_endEffectorPosition.setValue(newPosition);
        emit endEffectorPositionChanged();
    }
}

void Backend::updateMovingState() {
    // The moving state is handled by the binding, but we can emit additional signals here
    if (!isMoving()) {
        emit positionReached();
    }

    emit isMovingChanged();
}

void Backend::checkMotionComplete() {
    if (!isMoving()) {
        emit positionReached();
    }
}

QVector3D Backend::getJointPosition(int jointIndex) const {
    // Calculate position of specific joint
    // Implementation would depend on your specific needs
    return QVector3D();
}

QQuaternion Backend::getJointOrientation(int jointIndex) const {
    // Calculate orientation of specific joint
    // Implementation would depend on your specific needs
    return QQuaternion();
}
