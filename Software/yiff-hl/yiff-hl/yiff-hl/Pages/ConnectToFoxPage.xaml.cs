using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace yiff_hl.Pages
{
	[XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class ConnectToFoxPage : ContentPage
    {
        public ConnectToFoxPage()
        {
            InitializeComponent();
        }

        public async void OnOkClicked(object sender, EventArgs args)
        {
            await Navigation.PopModalAsync();
        }

        public async void OnCancelClicked(object sender, EventArgs args)
        {
            await Navigation.PopModalAsync();
        }

        protected override bool OnBackButtonPressed()
        {
            return base.OnBackButtonPressed();
        }
    }
}