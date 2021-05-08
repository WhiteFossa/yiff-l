using System;

namespace yiff_hl.Business.Helpers
{
    /// <summary>
    /// Helper to work with bits (converts int to bytes and so on)
    /// </summary>
    public static class BitsHelper
    {
        public static byte[] ConvertIntToBytes(int data)
        {
            byte[] result = BitConverter.GetBytes(data);
            if (BitConverter.IsLittleEndian)
            {
                Array.Reverse(result);
            }

            return result;
        }
    }
}
