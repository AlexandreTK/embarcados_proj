require 'test_helper'

class AlarmActionsControllerTest < ActionDispatch::IntegrationTest
  setup do
    @alarm_action = alarm_actions(:one)
  end

  test "should get index" do
    get alarm_actions_url
    assert_response :success
  end

  test "should get new" do
    get new_alarm_action_url
    assert_response :success
  end

  test "should create alarm_action" do
    assert_difference('AlarmAction.count') do
      post alarm_actions_url, params: { alarm_action: { disable_alarm: @alarm_action.disable_alarm } }
    end

    assert_redirected_to alarm_action_url(AlarmAction.last)
  end

  test "should show alarm_action" do
    get alarm_action_url(@alarm_action)
    assert_response :success
  end

  test "should get edit" do
    get edit_alarm_action_url(@alarm_action)
    assert_response :success
  end

  test "should update alarm_action" do
    patch alarm_action_url(@alarm_action), params: { alarm_action: { disable_alarm: @alarm_action.disable_alarm } }
    assert_redirected_to alarm_action_url(@alarm_action)
  end

  test "should destroy alarm_action" do
    assert_difference('AlarmAction.count', -1) do
      delete alarm_action_url(@alarm_action)
    end

    assert_redirected_to alarm_actions_url
  end
end
