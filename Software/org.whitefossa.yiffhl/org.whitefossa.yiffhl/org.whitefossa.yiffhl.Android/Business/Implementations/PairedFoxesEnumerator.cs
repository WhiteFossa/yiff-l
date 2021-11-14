using Android.Bluetooth;
using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Droid.Business.Implementations
{
    /// <summary>
    /// Paired foxes enumerator
    /// </summary>
    internal class PairedFoxesEnumerator : IPairedFoxesEnumerator
    {
        public async Task<IReadOnlyCollection<PairedFoxDTO>> EnumerateAsync()
        {
            var result = new List<PairedFoxDTO>();

            var adapter = BluetoothAdapter.DefaultAdapter;
            if (adapter == null)
            {
                throw new InvalidOperationException("No bluetooth adapters found!");
            }

            if (!adapter.IsEnabled)
            {
                throw new InvalidOperationException("Bluetooth is disabled!");
            }

            result.AddRange
            (
                adapter
                .BondedDevices
                .Select(d => new PairedFoxDTO(d.Name, d.Address))
            );

            return result;
        }
    }
}