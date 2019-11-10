class AddReminderToAlarm < ActiveRecord::Migration[5.2]
  def change
    add_column :alarms, :reminder, :boolean
  end
end
