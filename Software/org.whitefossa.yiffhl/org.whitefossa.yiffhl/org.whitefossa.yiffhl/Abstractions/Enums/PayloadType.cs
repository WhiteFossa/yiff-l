namespace org.whitefossa.yiffhl.Abstractions.Enums
{
    /// <summary>
    /// Possible packet payloads
    /// </summary>
    public enum PayloadType
    {
        CommandToFox = 0x00,

        ResponseToCommand = 0x01,

        EventFromFox = 0x02
    }
}
