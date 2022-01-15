using org.whitefossa.yiffhl.ViewModels;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace org.whitefossa.yiffhl.Views
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class ServicePageView : ContentPage
    {
        public ServicePageViewModel ViewModel
        {
            get => BindingContext as ServicePageViewModel;
            set => BindingContext = value;
        }

        public ServicePageView()
        {
            InitializeComponent();

            ViewModel.Navigation = Navigation;
        }

        protected override bool OnBackButtonPressed()
        {
            ViewModel.OnLeavingServiceDisplayAsync();

            return true;
        }

        public async Task OnShowAsync()
        {
            await ViewModel.OnShowAsync();
        }
    }
}