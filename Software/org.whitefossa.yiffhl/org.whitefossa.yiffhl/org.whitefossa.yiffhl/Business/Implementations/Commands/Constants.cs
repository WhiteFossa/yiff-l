namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    /// <summary>
    /// Constants, used in many commands
    /// </summary>
    public static class Constants
    {
        /// <summary>
        /// Profile maximal and minimal IDs
        /// </summary>
        public const int MinProfileId = 0;
        public const int MaxProfileId = 127;
        
        /// <summary>
        /// Maximal profiles count
        /// </summary>
        public const int MaxProfilesCount = MaxProfileId + 1;
    }
}
