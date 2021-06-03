namespace yiff_hl.Abstractions.Enums
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

        SwitchToProfile = 0x07
    }
}
