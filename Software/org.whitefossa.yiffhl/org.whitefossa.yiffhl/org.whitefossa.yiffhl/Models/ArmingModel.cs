using org.whitefossa.yiffhl.Abstractions.Enums;
using System;
using System.Collections.Generic;
using System.Text;

namespace org.whitefossa.yiffhl.Models
{
    public class ArmingModel
    {
        /// <summary>
        /// Arming status
        /// </summary>
        public ArmingStatus Status { get; set; }

        /// <summary>
        /// Current matching position (from 1)
        /// </summary>
        public int CurrentMatchingPosition { get; set; }

        /// <summary>
        /// Total matching positions
        /// </summary>
        public int TotalMatchingPositions { get; set; }
    }
}
