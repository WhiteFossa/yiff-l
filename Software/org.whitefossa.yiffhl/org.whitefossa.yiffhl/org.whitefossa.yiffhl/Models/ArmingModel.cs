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
        /// Antenna matching data
        /// </summary>
        public List<float> MatchingData { get; set; } = new List<float>();

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
