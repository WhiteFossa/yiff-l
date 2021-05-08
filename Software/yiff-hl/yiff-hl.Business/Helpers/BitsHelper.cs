using System;
using System.Collections.Generic;

namespace yiff_hl.Business.Helpers
{
    /// <summary>
    /// Helper to work with bits (converts int to bytes and so on)
    /// </summary>
    public static class BitsHelper
    {
        public static byte[] ConvertUInt32ToBytes(UInt32 data)
        {
            byte[] result = BitConverter.GetBytes(data);
            if (BitConverter.IsLittleEndian)
            {
                Array.Reverse(result);
            }

            return result;
        }

        public static UInt32 ConvertBytesToUint32(byte[] bytes)
        {
            if (bytes.Length != 4)
            {
                throw new ArgumentException("4 bytes are required", nameof(bytes));
            }

            if (BitConverter.IsLittleEndian)
            {
                Array.Reverse(bytes);
            }

            return BitConverter.ToUInt32(bytes, 0);
        }
    }
}
