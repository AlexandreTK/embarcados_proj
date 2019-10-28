class Alarm < ApplicationRecord


	def self.should_alarm_go_off(request_url)
		@setting = Setting.first
		timezone = @setting.timezone_hours_diff
		current_time = Time.now.utc + timezone.hours

		current_hour = current_time.hour
		current_min = current_time.min

		week_day_string = ""
		if (current_time.wday == 1)
			week_day_string = "repeat_monday"
		end
		if (current_time.wday == 2)
			week_day_string = "repeat_tuesday"
		end
		if (current_time.wday == 3)
			week_day_string = "repeat_wednesday"
		end
		if (current_time.wday == 4)
			week_day_string = "repeat_thursday"
		end
		if (current_time.wday == 5)
			week_day_string = "repeat_friday"
		end
		if (current_time.wday == 6)
			week_day_string = "repeat_saturday"
		end
		if (current_time.wday == 0)
			week_day_string = "repeat_sunday"
		end

		alarm_should_go_off = false
		alarm = Alarm.where(hour: current_hour, min:current_min).where( "#{week_day_string}": true )

		if alarm.count > 0
			alarm_should_go_off = true
		end

		alarm = Alarm.where(hour: current_hour, min:current_min).where( "#{week_day_string}": false ).where('created_at > ?', 24.hours.ago)
		
		if alarm.count > 0
			alarm_should_go_off = true
			alarm.each do |a|
				a.destroy
			end
		end




		str_return = "-1,-1,"
		if alarm_should_go_off
			if (@setting.operation_type == "Open Window and Play Song")
				str_return = "1,"+(@setting.minutes_to_next_action*60).to_s+","

				@last_action = AlarmAction.last
				if (@last_action)
					@last_action.disable_alarm = false
					@last_action.save
				end
			end
			if (@setting.operation_type == "Only Open Window")
				str_return = "1,-1,"
			end
			if (@setting.operation_type == "Only Play Song")
				str_return = "-1,1,"

				@last_action = AlarmAction.last
				if (@last_action)
					@last_action.disable_alarm = false
					@last_action.save
				end
			end
		end

		audio_url = nil
		if (@setting.mp3)
			audio_url = request_url + @setting.mp3.url
		end

		if (str_return == "-1,-1,")
			@last_action = AlarmAction.last
			if (@last_action)

				disable_alarm = @last_action.disable_alarm
				if (disable_alarm)
					@last_action.destroy
					str_return = "-1,-2,"
				end
			end
		end

		str_return = str_return + audio_url


		# "-1,1,audio_url" -> no_window, audio_next_second, audio_url
		# "1,1,audio_url"  -> window_next_second, audio_next_second, audio_url
		# "1,60,audio_url" -> window_next_second, audio_next_60_seconds, audio_url
		# "1,-1,audio_url" -> window_next_second, no_audio, audio_url

		return str_return #{enable: alarm_should_go_off, audio_url:audio}
	end


end
