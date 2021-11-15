using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace yiff_hl.Business.Helpers
{
    public static class CRCGenerator
    {
        private const UInt32 InitialValue = 0xFFFFFFFF;

        private const UInt32 Polynomial = 0x04C11DB7;

        /// <summary>
        /// STM-compatible CRC32 calculation (based on https://community.st.com/s/question/0D50X00009XkgnVSAR/crc32-calculation-in-c)
        /// </summary>
        public static UInt32 CalculateSTMCRC32(IReadOnlyCollection<UInt32> inputData)
        {
            UInt32 crc = InitialValue;
            foreach (var current in inputData)
            {
                crc ^= current;

                for (var bitIndex = 0; bitIndex < 32; bitIndex++)
                {
                    if ((crc & 0x80000000) != 0)
                    {
                        crc = ((crc << 1) ^ Polynomial);
                    }
                    else
                    {
                        crc <<= 1;
                    }
                }
            }
            return crc;
        }

        public static UInt32 CalculateSTMCRC32(IReadOnlyCollection<byte> inputData)
        {
            var resultBuffer = new List<UInt32>();
            var sizeIn32 = inputData.Count / 4;

            for (var i = 0; i < sizeIn32; i++)
            {
                var start = i * 4;
                var fourBytes = new byte[4];
                Array.Copy(inputData.ToArray(), start, fourBytes, 0, 4);

                resultBuffer.Add(BitsHelper.ConvertBytesToUint32(fourBytes));
            }

            var remainingBytes = inputData.Count % 4;
            if (remainingBytes != 0)
            {
                // Constructing last uint from bytes and alignment zeroes
                var lastUint = new byte[4] { 0, 0, 0, 0 };

                for (var i = 0; i < remainingBytes; i++)
                {
                    lastUint[i] = inputData.ElementAt(sizeIn32 * 4 + i);
                }

                resultBuffer.Add(BitsHelper.ConvertBytesToUint32(lastUint));
            }

            return CalculateSTMCRC32(resultBuffer);
        }
    }
}
