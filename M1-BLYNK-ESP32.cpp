
Initialize Blynk
Setup PIR Sensor
Setup Reed Switch


While (true) {
   
    If (PIR Sensor detects movement) {
        Trigger Alarm
        Send Alert to Blynk App
    }

   
    If (Reed Switch detects door/window opening) {
        Trigger Alarm
        Send Alert to Blynk App
    }

    
    Update Blynk App with current sensor status

    
    Wait (1 second)
}

