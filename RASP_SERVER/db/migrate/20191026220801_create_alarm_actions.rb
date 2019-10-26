class CreateAlarmActions < ActiveRecord::Migration[5.2]
  def change
    create_table :alarm_actions do |t|
      t.boolean :disable_alarm

      t.timestamps
    end
  end
end
