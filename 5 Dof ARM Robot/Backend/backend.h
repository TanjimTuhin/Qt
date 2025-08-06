// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef BACKEND_H
#define BACKEND_H

#include "animatedparam.h" // Assuming this is a custom class for handling animations
#include <QObject>
#include <qqmlregistration.h>
#include <QProperty> // Required for QProperty members
#include <QVector3D>
#include <QQuaternion>

//! [class definition]
class Backend : public QObject
{
    Q_OBJECT
    QML_ELEMENT // Exposes this class to QML

    // Properties to be controlled from QML (matches RoboticArm.qml and MainScreen.qml)
    Q_PROPERTY(int joint1Rotation READ joint1Rotation WRITE setJoint1Rotation NOTIFY joint1RotationChanged)
    Q_PROPERTY(int joint2Rotation READ joint2Rotation WRITE setJoint2Rotation NOTIFY joint2RotationChanged)
    Q_PROPERTY(int joint3Rotation READ joint3Rotation WRITE setJoint3Rotation NOTIFY joint3RotationChanged)
    Q_PROPERTY(int joint4Rotation READ joint4Rotation WRITE setJoint4Rotation NOTIFY joint4RotationChanged)
    Q_PROPERTY(int joint5Rotation READ joint5Rotation WRITE setJoint5Rotation NOTIFY joint5RotationChanged)
    Q_PROPERTY(int joint6Rotation READ joint6Rotation WRITE setJoint6Rotation NOTIFY joint6RotationChanged)
    Q_PROPERTY(int gripperAngle READ gripperAngle WRITE setGripperAngle NOTIFY gripperAngleChanged)

    // Read-only status property for the UI
    Q_PROPERTY(QString status READ status BINDABLE bindableStatus)

    // Additional properties for enhanced functionality
    Q_PROPERTY(bool isMoving READ isMoving NOTIFY isMovingChanged)
    Q_PROPERTY(bool hasCollision READ hasCollision NOTIFY hasCollisionChanged)
    Q_PROPERTY(QVector3D endEffectorPosition READ endEffectorPosition NOTIFY endEffectorPositionChanged)
    Q_PROPERTY(double workspaceRadius READ workspaceRadius CONSTANT)

    //! [class definition]

public:
    explicit Backend(QObject *parent = nullptr);

    // --- Getters ---
    int joint1Rotation() const;
    int joint2Rotation() const;
    int joint3Rotation() const;
    int joint4Rotation() const;
    int joint5Rotation() const;
    int joint6Rotation() const;
    int gripperAngle() const;
    QString status() const;
    QBindable<QString> bindableStatus() const;

    // Additional getters
    bool isMoving() const;
    bool hasCollision() const;
    QVector3D endEffectorPosition() const;
    double workspaceRadius() const { return 1.2; } // Maximum reach in meters

    // --- Setters (Slots) ---
public slots:
    void setJoint1Rotation(int angle);
    void setJoint2Rotation(int angle);
    void setJoint3Rotation(int angle);
    void setJoint4Rotation(int angle);
    void setJoint5Rotation(int angle);
    void setJoint6Rotation(int angle);
    void setGripperAngle(int angle);

    // Convenience methods for common operations
    void moveToHome();
    void moveToPickPosition();
    void moveToRestPosition();
    void moveToServicePosition();
    void openGripper();
    void closeGripper();
    void stopAllMotion();

    // Advanced control methods
    void setAllJoints(int j1, int j2, int j3, int j4, int j5, int j6);
    void setJointAnglesSmooth(int j1, int j2, int j3, int j4, int j5, int j6, int duration = 2000);

    // Safety and validation
    bool isPositionSafe(int j1, int j2, int j3, int j4, int j5, int j6) const;
    bool isWithinJointLimits(int joint, int angle) const;

signals:
    void joint1RotationChanged();
    void joint2RotationChanged();
    void joint3RotationChanged();
    void joint4RotationChanged();
    void joint5RotationChanged();
    void joint6RotationChanged();
    void gripperAngleChanged();
    void isMovingChanged();
    void hasCollisionChanged();
    void endEffectorPositionChanged();
    void emergencyStop();
    void positionReached();

private slots:
    void updateEndEffectorPosition();
    void checkMotionComplete();

private:
    // Member variables for each joint's angle
    AnimatedParam m_joint1Rotation;
    AnimatedParam m_joint2Rotation;
    AnimatedParam m_joint3Rotation;
    AnimatedParam m_joint4Rotation;
    AnimatedParam m_joint5Rotation;
    AnimatedParam m_joint6Rotation;
    AnimatedParam m_gripperAngle;

    // For status updates and collision detection
    QProperty<QString> m_status;
    QProperty<bool> m_isCollision;
    QProperty<bool> m_isMoving;
    QProperty<QVector3D> m_endEffectorPosition;

    // Joint limits for the new robotic arm (in degrees)
    struct JointLimits {
        int min;
        int max;
    };

    static const JointLimits s_jointLimits[6];

    // Private methods
    void detectCollision();
    void calculateForwardKinematics();
    bool checkSelfCollision() const;
    bool checkWorkspaceLimits() const;
    void updateMovingState();

    // Collision detection helpers for the new 32-component arm
    bool checkBaseCollisions() const;
    bool checkArmSegmentCollisions() const;
    bool checkWristCollisions() const;
    bool checkGripperCollisions() const;

    // Forward kinematics helper methods
    QVector3D getJointPosition(int jointIndex) const;
    QQuaternion getJointOrientation(int jointIndex) const;
};

#endif // BACKEND_H
