namespace org.whitefossa.yiffhl.Abstractions.Enums
{
    /// <summary>
    /// Possible commands
    /// </summary>
    public enum CommandType
    {
        SetDateAndTime = 0x00,

        SetFoxName = 0x01,

        GetFoxName = 0x02,

        GetProfilesCount = 0x03,

        GetProfileName = 0x04,

        AddNewProfile = 0x05,

        GetCurrentProfileId = 0x06,

        SwitchToProfile = 0x07,

        SetProfileName = 0x08,

        GetFrequency = 0x09,

        SetFrequency = 0x0A,

        GetCode = 0x0B,

        SetCode = 0x0C,

        GetSpeed = 0x0D,

        SetSpeed = 0x0E,

        GetCycle = 0x0F,

        SetCycle = 0x10,

        GetEndingToneDuration = 0x11,

        SetEndingToneDuration = 0x12,

        GetBeginAndEndTimes = 0x13,

        IsFoxArmed = 0x14,

        ArmFox = 0x15,

        DisarmFox = 0x16,

        SetBeginAndEndTimes = 0x17,

        GetFoxPower = 0x18,

        SetFoxPower = 0x19,

        GetBatteryLevel = 0x1A,

        GetUAntADC = 0x1B,

        GetUBattADC = 0x1C,

        GetU80mADC = 0x1D,

        GetUBattVolts = 0x1E,

        GetU80mVolts = 0x1F,

        GetLastFailureCode = 0x20,

        NoOperation = 0x21,

        GetIdentificationData = 0x22,

        GetAntennaMatchingStatus = 0x23,

        GetAntennaMatchingData = 0x24,

        MarkMatchingAsSeen = 0x25,

        CheckForProfileSettingsChanges = 0x26,

        ResetLastFailureCode = 0x27,

        UpdateSerialNumber = 0x28,

        GetUbattADCToUbattVoltsFactors = 0x29,

        SetUbattADCToUbattVoltsFactors = 0x2A
    }
}
