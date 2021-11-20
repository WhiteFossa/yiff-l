namespace org.whitefossa.yiffhl.Business.Helpers
{
    /// <summary>
    /// Common for many commands stuff
    /// </summary>
    public static class CommandsHelper
    {
        /// <summary>
        /// Determines if command was successful or not
        /// </summary>
        public static bool IsSuccessful(byte successByte)
        {
            return successByte == 0x00;
        }

        public static bool ToBool(byte data)
        {
            return data != 0x00;
        }

        public static byte FromBool(bool data)
        {
            return data ? (byte)0x01 : (byte)0x00;
        }
    }
}
