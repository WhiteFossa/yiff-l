namespace org.whitefossa.yiffhl.Abstractions.Enums
{
    /// <summary>
    /// Possible events
    /// </summary>
    public enum EventType
    {
        Armed = 0x01,

        AntennaMatchingMeasurement = 0x02,

        EnteringSleepmode = 0x03,

        ArmingInitiatedEvent = 0x04,

        Disarmed = 0x05,

        SettingsChanged = 0x06
    }
}
