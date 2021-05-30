using System;
using System.Collections.Generic;
using System.Text;

namespace yiff_hl.Business.Implementations.Commands.Helpers
{
    /// <summary>
    /// Common for many commands stuff
    /// </summary>
    public static class CommandsHelper
    {
        /// <summary>
        /// Determines if command was successful or not
        /// </summary>
        /// <param name="successByte"></param>
        /// <returns></returns>
        public static bool IsSuccessful(byte successByte)
        {
            return successByte == 0x00;
        }
    }
}
