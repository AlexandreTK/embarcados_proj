class WelcomeController < ApplicationController
  def index
  	@alarms = Alarm.all
  end
end
