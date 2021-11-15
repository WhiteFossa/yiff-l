namespace org.whitefossa.yiffhl.Abstractions.DTOs
{
    /// <summary>
    /// Represents a fox, paired to phone
    /// </summary>
    public class PairedFoxDTO
    {
        /// <summary>
        /// Bluetooth name
        /// </summary>
        public string Name { get; }

        /// <summary>
        /// MAC-address
        /// </summary>
        public string MAC { get; }

        public string DisplayName
        {
            get
            {
                return $"{ Name } MAC: { MAC }";
            }
        }

        public PairedFoxDTO(string name, string mac)
        {
            Name = name;
            MAC = mac;
        }
    }
}
