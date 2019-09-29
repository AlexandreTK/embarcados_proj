class WelcomeController < ApplicationController
  def index
  	@alarms = Alarm.all
  	@setting = Setting.first
  end
end
