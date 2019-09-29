json.extract! alarm, :id, :hour, :min, :repeat_monday, :repeat_tuesday, :repeat_wednesday, :repeat_thursday, :repeat_friday, :repeat_saturday, :repeat_sunday, :created_at, :updated_at
json.url alarm_url(alarm, format: :json)
