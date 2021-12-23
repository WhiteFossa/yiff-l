using org.whitefossa.yiffhl.Abstractions.Enums;
using System;
using System.Collections.Generic;
using System.Linq;
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

        /// <summary>
        /// Antenna matching data
        /// </summary>
        public Dictionary<int, float> MatchingData { get; set; } = new Dictionary<int, float>();

        /// <summary>
        /// Ordered by matcher position MatchingData
        /// </summary>
        public IOrderedEnumerable<KeyValuePair<int, float>> OrderdMatchingData { get; set; }

        /// <summary>
        /// When antenna mather in this position the antenna voltage is highest
        /// </summary>
        public int BestMatchingPosition { get; set; }

        /// <summary>
        /// Antenna voltage at best matching position
        /// </summary>
        public float BestMatchingPositionVoltage { get; set; }
    }
}
