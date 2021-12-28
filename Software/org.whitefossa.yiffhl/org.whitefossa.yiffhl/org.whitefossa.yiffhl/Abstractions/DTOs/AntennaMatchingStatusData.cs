using org.whitefossa.yiffhl.Abstractions.Enums;
using System;

namespace org.whitefossa.yiffhl.Abstractions.DTOs
{
    public class AntennaMatchingStatusData
    {
        /// <summary>
        /// Antenna matching status
        /// </summary>
        public AntennaMatchingStatus Status { get; set; }

        /// <summary>
        /// Time since last initiation
        /// </summary>
        public TimeSpan TimeSinceLastInitiation { get; set; }

        /// <summary>
        /// Total matcher positions
        /// </summary>
        public int TotalMatcherPositions { get; set; }

        /// <summary>
        /// Current matcher position (counted from 0)
        /// </summary>
        public int CurrentMatcherPosition { get; set; }

        /// <summary>
        /// Current best matcher position (counted from 0)
        /// </summary>
        public int CurrentBestMatchPosition { get; set; }

        /// <summary>
        /// Current best voltage
        /// </summary>
        public float CurrentBestMatchVoltage { get; set; }
    }
}
