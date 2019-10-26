require 'test_helper'

class AlarmsControllerTest < ActionDispatch::IntegrationTest
  setup do
    @alarm = alarms(:one)
  end

  test "should get index" do
    get alarms_url
    assert_response :success
  end

  test "should get new" do
    get new_alarm_url
    assert_response :success
  end

  test "should create alarm" do
    assert_difference('Alarm.count') do
      post alarms_url, params: { alarm: { hour: @alarm.hour, min: @alarm.min, repeat_friday: @alarm.repeat_friday, repeat_monday: @alarm.repeat_monday, repeat_saturday: @alarm.repeat_saturday, repeat_sunday: @alarm.repeat_sunday, repeat_thursday: @alarm.repeat_thursday, repeat_tuesday: @alarm.repeat_tuesday, repeat_wednesday: @alarm.repeat_wednesday } }
    end

    assert_redirected_to alarm_url(Alarm.last)
  end

  test "should show alarm" do
    get alarm_url(@alarm)
    assert_response :success
  end

  test "should get edit" do
    get edit_alarm_url(@alarm)
    assert_response :success
  end

  test "should update alarm" do
    patch alarm_url(@alarm), params: { alarm: { hour: @alarm.hour, min: @alarm.min, repeat_friday: @alarm.repeat_friday, repeat_monday: @alarm.repeat_monday, repeat_saturday: @alarm.repeat_saturday, repeat_sunday: @alarm.repeat_sunday, repeat_thursday: @alarm.repeat_thursday, repeat_tuesday: @alarm.repeat_tuesday, repeat_wednesday: @alarm.repeat_wednesday } }
    assert_redirected_to alarm_url(@alarm)
  end

  test "should destroy alarm" do
    assert_difference('Alarm.count', -1) do
      delete alarm_url(@alarm)
    end

    assert_redirected_to alarms_url
  end
end
