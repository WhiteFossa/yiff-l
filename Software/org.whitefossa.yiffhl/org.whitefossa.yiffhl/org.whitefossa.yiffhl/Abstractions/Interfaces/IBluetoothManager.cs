namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    /// <summary>
    /// Interface to manage bluetooth devices
    /// </summary>
    public interface IBluetoothManager
    {
        /// <summary>
        /// Unpairs paired device by MAC address
        /// </summary>
        void UnPair(string mac);
    }
}
