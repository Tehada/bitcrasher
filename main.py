from six.moves import urllib
import tensorflow as tf

FLAGS = tf.app.flags.FLAGS

# Model parameters
tf.app.flags.DEFINE_integer('batch_size', 128,
                            """Number of images to process in a batch.""")
tf.app.flags.DEFINE_string('data_dir', '/tmp/dibco_data',
                           """Path to the DIBCO data directory.""")
tf.app.flags.DEFINE_boolean('use_fp16', False,
                            """Train the model using fp16.""")

# DIBCO data set.
IMAGE_SIZE = dibco_input.IMAGE_SIZE
NUM_CLASSES = dibco_input.NUM_CLASSES
NUM_EXAMPLES_PER_EPOCH_FOR_TRAIN = dibco_input.NUM_EXAMPLES_PER_EPOCH_FOR_TRAIN
NUM_EXAMPLES_PER_EPOCH_FOR_EVAL = dibco_input.NUM_EXAMPLES_PER_EPOCH_FOR_EVAL

TOWER_NAME = 'tower'

DATA_URL = 'http://users.iit.demokritos.gr/~bgat/HDIBCO2014/benchmark/dataset/original_images.rar'


def _activation_summary(x):
  tensor_name = re.sub('%s_[0-9]*/' % TOWER_NAME, '', x.op.name)
  tf.contrib.deprecated.histogram_summary(tensor_name + '/activations', x)
  tf.contrib.deprecated.scalar_summary(tensor_name + '/sparsity',
                                       tf.nn.zero_fraction(x))


def _variable_on_cpu(name, shape, initializer):
  with tf.device('/cpu:0'):
    dtype = tf.float16 if FLAGS.use_fp16 else tf.float32
    var = tf.get_variable(name, shape, initializer=initializer, dtype=dtype)
  return var


def _variable_with_weight_decay(name, shape, stddev, wd):
  dtype = tf.float16 if FLAGS.use_fp16 else tf.float32
  var = _variable_on_cpu(
      name,
      shape,
      tf.truncated_normal_initializer(stddev=stddev, dtype=dtype))
  if wd is not None:
    weight_decay = tf.mul(tf.nn.l2_loss(var), wd, name='weight_loss')
    tf.add_to_collection('losses', weight_decay)
  return var