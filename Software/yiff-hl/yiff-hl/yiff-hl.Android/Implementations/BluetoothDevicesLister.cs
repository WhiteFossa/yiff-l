using Android.Bluetooth;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.DTOs;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Droid.Implementations
{
    public class BluetoothDevicesLister : IBluetoothDevicesLister
    {
        IReadOnlyCollection<BluetoothDeviceDTO> IBluetoothDevicesLister.ListPairedDevices()
        {
            var result = new List<BluetoothDeviceDTO>();

            var adapter = BluetoothAdapter.DefaultAdapter;
            if (adapter == null)
            {
                System.Diagnostics.Debug.WriteLine("No bluetooth adapters found.");
                return result;
            }

            if (!adapter.IsEnabled)
            {
                System.Diagnostics.Debug.WriteLine("Bluetooth adapter is disabled.");
                return result;
            }

            result.AddRange(adapter
                .BondedDevices
                .Select(d => new BluetoothDeviceDTO(d.Name, d.Address)));

            return result;
        }
    }
}