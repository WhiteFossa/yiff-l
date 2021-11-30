using org.whitefossa.yiffhl.Abstractions.Enums;
using System;

namespace org.whitefossa.yiffhl.Abstractions.DTOs
{
    /// <summary>
    /// Fox code
    /// </summary>
    public class Callsign
    {
        public FoxCode Code { get; set; }

        /// <summary>
        /// Callsign name
        /// </summary>
        public string Name
        {
            get
            {
                switch (Code)
                {
                    case FoxCode.Finish:
                        return "Finish (MO)";

                    case FoxCode.Fox1:
                        return "Fox #1 (MOE)";

                    case FoxCode.Fox2:
                        return "Fox #2 (MOI)";

                    case FoxCode.Fox3:
                        return "Fox #3 (MOS)";

                    case FoxCode.Fox4:
                        return "Fox #4 (MOH)";

                    case FoxCode.Fox5:
                        return "Fox #5 (MO5)";

                    case FoxCode.Beacon:
                        return "Beacon (S)";

                    default:
                        throw new ArgumentException(nameof(Code));
                }
            }
        }
    }
}
