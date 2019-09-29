Rails.application.routes.draw do
  resources :settings
  #get 'welcome/index'
  root 'welcome#index'
  resources :alarms
  # For details on the DSL available within this file, see http://guides.rubyonrails.org/routing.html
end
