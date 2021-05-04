using System.Collections.ObjectModel;

namespace yiff_hl.Data
{
    /// <summary>
    /// Model for Connect to fox page
    /// </summary>
    public class ConnectToFoxModel
    {
        private ObservableCollection<BluetoothDevice> bluetoothDevices = new ObservableCollection<BluetoothDevice>();

        public ObservableCollection<BluetoothDevice> BluetoothDevices { get { return bluetoothDevices; } }
    }
}
