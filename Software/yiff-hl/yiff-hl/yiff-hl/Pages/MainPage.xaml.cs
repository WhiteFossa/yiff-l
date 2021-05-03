using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Forms;

namespace yiff_hl.Pages
{
    public partial class MainPage : ContentPage
    {
        public MainPage()
        {
            InitializeComponent();

            btnConnectToFox.Clicked += (s, e) => Navigation.PushModalAsync(new ConnectToFoxPage());
        }
    }
}
