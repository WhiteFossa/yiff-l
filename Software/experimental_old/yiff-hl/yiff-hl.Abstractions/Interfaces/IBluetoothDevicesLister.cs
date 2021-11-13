using System.Collections.Generic;
using yiff_hl.Abstractions.DTOs;

namespace yiff_hl.Abstractions.Interfaces
{
    /// <summary>
    /// Use this to get list of bluetooth devices
    /// </summary>
    public interface IBluetoothDevicesLister
    {
        IReadOnlyCollection<BluetoothDeviceDTO> ListPairedDevices();
    }
}
