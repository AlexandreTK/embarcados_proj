class WelcomeController < ApplicationController
  def index
  	@alarms = Alarm.all
  	@setting = Setting.first
  end

  def get_alarm
  	render json: Alarm.should_alarm_go_off(request.base_url)
  end
end
