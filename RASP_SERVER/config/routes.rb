Rails.application.routes.draw do
  resources :alarm_actions
  resources :settings
  #get 'welcome/index'
  root 'welcome#index'
  resources :alarms

  get 'get_alarm', to: "welcome#get_alarm"
  # For details on the DSL available within this file, see http://guides.rubyonrails.org/routing.html
end
