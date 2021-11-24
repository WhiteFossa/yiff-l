using org.whitefossa.yiffhl.ViewModels;
using System;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace org.whitefossa.yiffhl.Views
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class MainPageView : ContentPage
    {
        MainPageViewModel ViewModel
        {
            get => BindingContext as MainPageViewModel;
            set => BindingContext = value;
        }

        public MainPageView()
        {
            InitializeComponent();
        }

        private void pkFox_SelectedIndexChanged(object sender, EventArgs e)
        {
            var selectedFox = (sender as Picker).SelectedItem;

            ViewModel.SelectedFoxChangedCommand.Execute(selectedFox);
        }

        private void pkProfile_SelectedIndexChanged(object sender, EventArgs e)
        {
            var selectedProfile = (sender as Picker).SelectedItem;

            ViewModel.SelectedProfileChangedCommand.Execute(selectedProfile);
        }
    }
}