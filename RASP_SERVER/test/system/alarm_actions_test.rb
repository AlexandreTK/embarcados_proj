require "application_system_test_case"

class AlarmActionsTest < ApplicationSystemTestCase
  setup do
    @alarm_action = alarm_actions(:one)
  end

  test "visiting the index" do
    visit alarm_actions_url
    assert_selector "h1", text: "Alarm Actions"
  end

  test "creating a Alarm action" do
    visit alarm_actions_url
    click_on "New Alarm Action"

    check "Disable alarm" if @alarm_action.disable_alarm
    click_on "Create Alarm action"

    assert_text "Alarm action was successfully created"
    click_on "Back"
  end

  test "updating a Alarm action" do
    visit alarm_actions_url
    click_on "Edit", match: :first

    check "Disable alarm" if @alarm_action.disable_alarm
    click_on "Update Alarm action"

    assert_text "Alarm action was successfully updated"
    click_on "Back"
  end

  test "destroying a Alarm action" do
    visit alarm_actions_url
    page.accept_confirm do
      click_on "Destroy", match: :first
    end

    assert_text "Alarm action was successfully destroyed"
  end
end
