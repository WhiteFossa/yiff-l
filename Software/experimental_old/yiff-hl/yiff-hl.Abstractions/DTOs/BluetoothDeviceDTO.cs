namespace yiff_hl.Abstractions.DTOs
{
    public class BluetoothDeviceDTO
    {
        public BluetoothDeviceDTO(string name, string mac)
        {
            Name = name;
            MAC = mac;
        }

        public string Name { get; }

        public string MAC { get; }
    }
}
