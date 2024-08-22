// Initialize Blynk and Sensors
Initialize Blynk
Setup PIR Sensor
Setup Reed Switch

// Main Loop
While (true) {
    // Read PIR Sensor
    If (PIR Sensor detects movement) {
        Trigger Alarm
        Send Alert to Blynk App
    }

    // Read Reed Switch
    If (Reed Switch detects door/window opening) {
        Trigger Alarm
        Send Alert to Blynk App
    }

    // Update Blynk App with sensor status
    Update Blynk App with current sensor status

    // Wait for a short period before checking again
    Wait (1 second)
}

