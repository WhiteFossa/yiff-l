using System.Collections.Generic;

namespace org.whitefossa.yiffhl.Models
{
    public class MatchingModel
    {
        /// <summary>
        /// Matching positions count
        /// </summary>
        public int MatchingPositionsCount { get; set; }

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
