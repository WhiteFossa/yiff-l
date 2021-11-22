using Android.Bluetooth;
using Java.Lang;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using System;

namespace org.whitefossa.yiffhl.Droid.Business.Implementations
{
    public class BluetoothManager : IBluetoothManager
    {
        public void UnPair(string mac)
        {
            var adapter = BluetoothAdapter.DefaultAdapter;
            if (adapter == null)
            {
                throw new InvalidOperationException("No bluetooth adapters found!");
            }

            var device = adapter.GetRemoteDevice(mac);
            if (device == null)
            {
                throw new ArgumentException($"Device with MAC { mac } is not found!");
            }

            try
            {
                var methodInfo = device.Class.GetMethod("removeBond", (Class[])null);
                methodInfo.Invoke(device, null);
            }
            catch(System.Exception ex)
            {
                // We can do nothing here in case of failure
                // TODO: Log a failure
            }
        }
    }
}