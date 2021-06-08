﻿namespace yiff_hl.Abstractions.Enums
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

        IsFoxArmed = 0x14
    }
}
