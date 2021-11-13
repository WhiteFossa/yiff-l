using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

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

        public PairedFoxDTO(string name, string mac)
        {
            Name = name;
            MAC = mac;
        }
    }
}