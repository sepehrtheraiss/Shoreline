defmodule INode do
  use Agent

  def start_link(node_id) do
    Agent.start_link(fn -> [-1, node_id] end, name: __MODULE__)
  end

  def sequence do
    Agent.update(__MODULE__, & [hd(&1) + 1, List.last(&1)])
    seq = Agent.get(__MODULE__, & hd(&1))
    if seq  >= 4096 do
     Agent.update(__MODULE__, & [0, hd(&1)])
    end
    Agent.get(__MODULE__, & hd(&1)) 
  end

  def node_id do
    Agent.get(__MODULE__,& List.last(&1))
  end
end

defmodule GlobalId do
  use Bitwise

  @moduledoc """
  GlobalId module contains an implementation of a guaranteed globally unique id system.     
  """

  @doc """
  Please implement the following function.
  64 bit non negative integer output   
  """
  @spec get_id() :: String  
  def get_id() do
    t = timestamp() <<< (63-41) 
    id = node_id() <<< (63-41-10)
    seq = INode.sequence()

    IO.puts "node id: #{node_id()}" 
    IO.puts "seq: #{seq}" 

    uid = t ||| id ||| seq
    if byte_size(:binary.encode_unsigned(uid)) != 8 do
      {:ok, file} = File.open("log", [:append])
      IO.binwrite(file, "Error: Invalid UUID #{uid}\n" )
      File.close(file)
    end

    Integer.to_string(uid)
  end

  #
  # You are given the following helper functions
  # Presume they are implemented - there is no need to implement them. 
  #

  @doc """
  Returns your node id as an integer.
  It will be greater than or equal to 0 and less than or equal to 1024.
  It is guaranteed to be globally unique. 
  """
  @spec node_id() :: non_neg_integer
  def node_id() do
    INode.node_id()
  end

  @doc """
  Returns timestamp since the epoch in milliseconds. 
  """
  @spec timestamp() :: non_neg_integer
  def timestamp() do 
    :os.system_time(:milli_seconds)
  end
end

node_id = String.to_integer(hd(System.argv()))
if node_id < 1024 do 
  INode.start_link(node_id)

  IO.puts GlobalId.get_id()
  IO.puts GlobalId.get_id()
  IO.puts GlobalId.get_id()
else
  IO.puts "node id range 0 - 1023"
end
