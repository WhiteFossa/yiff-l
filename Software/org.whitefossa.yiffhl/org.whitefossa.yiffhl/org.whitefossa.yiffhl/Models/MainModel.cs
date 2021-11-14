using org.whitefossa.yiffhl.Abstractions.DTOs;

namespace org.whitefossa.yiffhl.Models
{
    /// <summary>
    /// Main model
    /// </summary>
    public class MainModel
    {
        /// <summary>
        /// Are we connected to a fox?
        /// </summary>
        public bool IsConnected { get; set; }

        /// <summary>
        /// We are connected to this fox (if connected at all)
        /// </summary>
        public PairedFoxDTO ConnectedFox { get; set; }

        public MainModel()
        {
            // Initial state
            IsConnected = false;
            ConnectedFox = null;
        }
    }
}
