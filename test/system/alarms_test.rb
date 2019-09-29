require "application_system_test_case"

class AlarmsTest < ApplicationSystemTestCase
  setup do
    @alarm = alarms(:one)
  end

  test "visiting the index" do
    visit alarms_url
    assert_selector "h1", text: "Alarms"
  end

  test "creating a Alarm" do
    visit alarms_url
    click_on "New Alarm"

    fill_in "Hour", with: @alarm.hour
    fill_in "Min", with: @alarm.min
    check "Repeat friday" if @alarm.repeat_friday
    check "Repeat monday" if @alarm.repeat_monday
    check "Repeat saturday" if @alarm.repeat_saturday
    check "Repeat sunday" if @alarm.repeat_sunday
    check "Repeat thursday" if @alarm.repeat_thursday
    check "Repeat tuesday" if @alarm.repeat_tuesday
    check "Repeat wednesday" if @alarm.repeat_wednesday
    click_on "Create Alarm"

    assert_text "Alarm was successfully created"
    click_on "Back"
  end

  test "updating a Alarm" do
    visit alarms_url
    click_on "Edit", match: :first

    fill_in "Hour", with: @alarm.hour
    fill_in "Min", with: @alarm.min
    check "Repeat friday" if @alarm.repeat_friday
    check "Repeat monday" if @alarm.repeat_monday
    check "Repeat saturday" if @alarm.repeat_saturday
    check "Repeat sunday" if @alarm.repeat_sunday
    check "Repeat thursday" if @alarm.repeat_thursday
    check "Repeat tuesday" if @alarm.repeat_tuesday
    check "Repeat wednesday" if @alarm.repeat_wednesday
    click_on "Update Alarm"

    assert_text "Alarm was successfully updated"
    click_on "Back"
  end

  test "destroying a Alarm" do
    visit alarms_url
    page.accept_confirm do
      click_on "Destroy", match: :first
    end

    assert_text "Alarm was successfully destroyed"
  end
end
