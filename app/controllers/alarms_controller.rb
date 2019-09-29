class AlarmsController < ApplicationController
  before_action :set_alarm, only: [:show, :edit, :update, :destroy]

  # GET /alarms
  # GET /alarms.json
  def index
    @alarms = Alarm.all
  end

  # GET /alarms/1
  # GET /alarms/1.json
  def show
  end

  # GET /alarms/new
  def new
    @alarm = Alarm.new
  end

  # GET /alarms/1/edit
  def edit
  end

  # POST /alarms
  # POST /alarms.json
  def create
    # puts("******PARAMS:")
    # puts(params)
    # puts(params[:alarm])
    # puts(params[:alarm][:from])
    # #params.has_key?("hour") ? puts(params[:hour]) : puts("no hour")

    @alarm = Alarm.new(alarm_params)

    #@user_id = params.has_key?("user_id") ? params[:user_id] | current_user.id

    

    if params[:alarm][:from]=='home'
      if @alarm.save
        redirect_to root_path
      else
        redirect_to root_path
      end
    else
      respond_to do |format|
        if @alarm.save
          format.html { redirect_to @alarm, notice: 'Alarm was successfully created.' }
          format.json { render :show, status: :created, location: @alarm }
        else
          format.html { render :new }
          format.json { render json: @alarm.errors, status: :unprocessable_entity }
        end
      end
    end


  end

  # PATCH/PUT /alarms/1
  # PATCH/PUT /alarms/1.json
  def update
    respond_to do |format|
      if @alarm.update(alarm_params)
        format.html { redirect_to @alarm, notice: 'Alarm was successfully updated.' }
        format.json { render :show, status: :ok, location: @alarm }
      else
        format.html { render :edit }
        format.json { render json: @alarm.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /alarms/1
  # DELETE /alarms/1.json
  def destroy
    @alarm.destroy

    if params[:from]=='home'
      redirect_to root_path
    else
      respond_to do |format|
        format.html { redirect_to alarms_url, notice: 'Alarm was successfully destroyed.' }
        format.json { head :no_content }
      end
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_alarm
      @alarm = Alarm.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def alarm_params
      params.require(:alarm).permit(:hour, :min, :repeat_monday, :repeat_tuesday, :repeat_wednesday, :repeat_thursday, :repeat_friday, :repeat_saturday, :repeat_sunday)
    end
end
