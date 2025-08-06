import QtQuick
import QtQuick3D

Node {
    id: roboticArm

        // Add this line to rotate the entire robot assembly
            eulerRotation.x: -90


    // Control properties for 6 degrees of freedom
    property real joint1Rotation: 0    // Base rotation (Z-axis) - 360° vertical rotation
    property real joint2Rotation: 0    // Shoulder pitch (Y-axis) - (-90, 90)
    property real joint3Rotation: 0    // Elbow pitch (Y-axis) - (-135, 135)
    property real joint4Rotation: 0    // Wrist roll (Z-axis) - (-180, 180)
    property real joint5Rotation: 0    // Wrist pitch (Y-axis) - (-90, 90)
    property real joint6Rotation: 0    // Wrist yaw (Y-axis) - (-180, 180)
    property real gripperAngle: 0      // Gripper open/close (0-30 degrees)

    // Position aliases for inverse kinematics
    readonly property alias basePosition: baseAssembly.scenePosition
    readonly property alias joint1Position: joint1Assembly.scenePosition
    readonly property alias joint2Position: joint2Assembly.scenePosition
    readonly property alias joint3Position: joint3Assembly.scenePosition
    readonly property alias joint4Position: joint4Assembly.scenePosition
    readonly property alias joint5Position: joint5Assembly.scenePosition
    readonly property alias joint6Position: joint6Assembly.scenePosition
    readonly property alias gripperPosition: gripperAssembly.scenePosition

    // Materials with distinct colors matching the image
    PrincipledMaterial {
        id: baseMaterial
        baseColor: "#4FC3F7"  // Light blue for base parts
        metalness: 0.8
        roughness: 0.2
    }

    PrincipledMaterial {
        id: joint1Material
        baseColor: "#F44336"  // Red for joint 1 (first rotational)
        metalness: 0.6
        roughness: 0.3
    }

    PrincipledMaterial {
        id: joint2Material
        baseColor: "#424242"  // Dark gray for joint 2
        metalness: 0.7
        roughness: 0.25
    }

    PrincipledMaterial {
        id: joint3Material
        baseColor: "#9C27B0"  // Purple for joint 3
        metalness: 0.6
        roughness: 0.3
    }

    PrincipledMaterial {
        id: joint4Material
        baseColor: "#673AB7"  // Deep purple for joint 4
        metalness: 0.6
        roughness: 0.3
    }

    PrincipledMaterial {
        id: joint5Material
        baseColor: "#E91E63"  // Pink for joint 5
        metalness: 0.6
        roughness: 0.3
    }

    PrincipledMaterial {
        id: joint6Material
        baseColor: "#4CAF50"  // Green for joint 6 (end effector)
        metalness: 0.5
        roughness: 0.4
    }

    PrincipledMaterial {
        id: controllerBoxMaterial
        baseColor: "#2196F3"  // Blue for controller box
        metalness: 0.8
        roughness: 0.2
    }

    // BASE ASSEMBLY (Stationary Object)
    Node {
        id: baseAssembly

        // Base component
        Model {
            source: "meshes/Base.mesh"
            materials: [baseMaterial]
        }

        // JOINT 1 ASSEMBLY (First rotational object - 360° vertical rotation)
        Node {
            id: joint1Assembly
            eulerRotation.z: joint1Rotation

            // Art1 component (first rotational part)
            Model {
                source: "meshes/art1.mesh"
                materials: [joint1Material]
                y: 0.05  // Slight elevation from base
            }

            // JOINT 2 ASSEMBLY (Shoulder pitch)
            Node {
                id: joint2Assembly
                z:135
                y: 0  // Height offset based on technical specifications
                eulerRotation.y: joint2Rotation

                // Art2 component (shoulder link)
                Model {
                    source: "meshes/art2.mesh"
                    materials: [joint2Material]
                }

                // JOINT 3 ASSEMBLY (Elbow pitch)
                Node {
                    id: joint3Assembly
                    y: 0.15   // Height offset for elbow joint
                    z: 150   // Forward offset
                    eulerRotation.y: joint3Rotation

                    // Art3 component (elbow link)
                    Model {
                        source: "meshes/art3.mesh"
                        materials: [joint3Material]
                    }

                    // JOINT 4 ASSEMBLY (Wrist roll - Z axis)
                    Node {
                        id: joint4Assembly
                        y: 0.12  // Height offset for wrist
                        z:60
                        eulerRotation.z: joint4Rotation

                        // Art4 component (wrist roll)
                        Model {
                            source: "meshes/art4.mesh"
                            materials: [joint4Material]
                        }

                        // JOINT 5 ASSEMBLY (Wrist pitch - X axis)
                        Node {
                            id: joint5Assembly
                            y: 0.06  // Small offset
                            z:70
                            eulerRotation.y: joint5Rotation

                            // Art5 component (wrist pitch)
                            Model {
                                source: "meshes/art5.mesh"
                                materials: [joint5Material]
                            }

                            // JOINT 6 ASSEMBLY (Wrist yaw - Y axis)
                            Node {
                                id: joint6Assembly
                                y: 0.05  // Small offset
                                z:35
                                eulerRotation.y: joint6Rotation

                                // Art6 component (end effector base)
                                Model {
                                    source: "meshes/art6.mesh"
                                    materials: [joint6Material]
                                }

                                // GRIPPER ASSEMBLY
                                Node {
                                    id: gripperAssembly
                                    y: 0.04  // Height for gripper attachment

                                    // Gripper base (static part)
                                    Model {
                                        id: gripperBase
                                        source: "meshes/gripper_base.mesh"
                                        materials: [joint6Material]
                                    }

                                    // Left gripper finger
                                    Model {
                                        id: leftGripperFinger
                                        x: -0.015
                                        y: 0.02
                                        eulerRotation.z: -gripperAngle
                                        source: "meshes/gripper_finger.mesh"
                                        materials: [joint6Material]
                                    }

                                    // Right gripper finger
                                    Model {
                                        id: rightGripperFinger
                                        x: 0.015
                                        y: 0.02
                                        eulerRotation.z: gripperAngle
                                        source: "meshes/gripper_finger.mesh"
                                        materials: [joint6Material]
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Controller Box (Stationary Object) - positioned beside the base
    Node {
        id: controllerBox
        x: -0.3  // Position to the side of the robot
        z: 0.1   // Slightly forward

        Model {
            source: "meshes/controller_box.mesh"
            materials: [controllerBoxMaterial]
        }
    }

    // Animation components for smooth movement
    PropertyAnimation {
        id: joint1Animation
        target: roboticArm
        property: "joint1Rotation"
        duration: 2000
        easing.type: Easing.InOutQuad
    }

    PropertyAnimation {
        id: joint2Animation
        target: roboticArm
        property: "joint2Rotation"
        duration: 2000
        easing.type: Easing.InOutQuad
    }

    PropertyAnimation {
        id: joint3Animation
        target: roboticArm
        property: "joint3Rotation"
        duration: 2000
        easing.type: Easing.InOutQuad
    }

    PropertyAnimation {
        id: joint4Animation
        target: roboticArm
        property: "joint4Rotation"
        duration: 2000
        easing.type: Easing.InOutQuad
    }

    PropertyAnimation {
        id: joint5Animation
        target: roboticArm
        property: "joint5Rotation"
        duration: 2000
        easing.type: Easing.InOutQuad
    }

    PropertyAnimation {
        id: joint6Animation
        target: roboticArm
        property: "joint6Rotation"
        duration: 2000
        easing.type: Easing.InOutQuad
    }

    PropertyAnimation {
        id: gripperAnimation
        target: roboticArm
        property: "gripperAngle"
        duration: 1000
        easing.type: Easing.InOutQuad
    }

    // Helper functions for controlling the robot
    function moveToPosition(j1, j2, j3, j4, j5, j6, gripperPos) {
        // Clamp values to joint limits
        joint1Animation.to = Math.max(-180, Math.min(180, j1))
        joint2Animation.to = Math.max(-90, Math.min(90, j2))
        joint3Animation.to = Math.max(-135, Math.min(135, j3))
        joint4Animation.to = Math.max(-180, Math.min(180, j4))
        joint5Animation.to = Math.max(-90, Math.min(90, j5))
        joint6Animation.to = Math.max(-180, Math.min(180, j6))
        gripperAnimation.to = Math.max(0, Math.min(30, gripperPos))

        joint1Animation.start()
        joint2Animation.start()
        joint3Animation.start()
        joint4Animation.start()
        joint5Animation.start()
        joint6Animation.start()
        gripperAnimation.start()
    }

    function homePosition() {
        moveToPosition(0, 0, 0, 0, 0, 0, 0)
    }

    function openGripper() {
        gripperAnimation.to = 30
        gripperAnimation.start()
    }

    function closeGripper() {
        gripperAnimation.to = 0
        gripperAnimation.start()
    }

    // Predefined poses
    function reachPose() {
        moveToPosition(45, -30, 45, 0, 30, 0, 15)
    }

    function retractPose() {
        moveToPosition(0, 60, -90, 0, 30, 0, 0)
    }

    function sidePose() {
        moveToPosition(90, 0, 0, 90, 0, 0, 15)
    }

    // Validation function for joint limits
    function validateJointLimits() {
        return {
            joint1: { min: -180, max: 180, current: joint1Rotation },
            joint2: { min: -90, max: 90, current: joint2Rotation },
            joint3: { min: -135, max: 135, current: joint3Rotation },
            joint4: { min: -180, max: 180, current: joint4Rotation },
            joint5: { min: -90, max: 90, current: joint5Rotation },
            joint6: { min: -180, max: 180, current: joint6Rotation },
            gripper: { min: 0, max: 30, current: gripperAngle }
        }
    }
}
